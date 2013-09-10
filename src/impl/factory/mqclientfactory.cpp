#include "mqclientfactory.h"
#include "common\mixall.h"
#include "mqclientapiimpl.h"
#include "producer\defaultmqproducer.h"
#include "impl\producer\defaultmqproducerimpl.h"
#include "topicpublishinfo.h"
#include "protocol\topicroutedata.h"
#include "protocol\heartbeatdata.h"
#include <Windows.h>
#include "impl\consumer\defaultmqpullconsumerimpl.h"
#include "impl\findbrokerresult.h"


MQClientFactory::MQClientFactory( int factoryIndex, std::string clientId ) : serviceState(ServiceState::CREATE_JUST), bootTimestamp(123), LockTimeout(3)
{
	this->factoryIndex = factoryIndex;
	this->clientId = clientId;

	this->mQClientAPIImpl = new MQClientAPIImpl();
	//update nameserver address
	std::string namerserver = this->mQClientAPIImpl->fetchNameServerAddr();
	this->mQClientAPIImpl->updateNameServerAddressList(namerserver);
	this->defaultMQProducer = new DefaultMQProducer();
}

MQClientFactory::~MQClientFactory()
{
	delete this->defaultMQProducer;
}

void MQClientFactory::start() {
	if(this->serviceState == ServiceState::CREATE_JUST) {
		this->serviceState = ServiceState::RUNNING;
		this->mQClientAPIImpl->start();
		this->defaultMQProducer->getDefaultMQProducerImpl()->start(false);
		this->startScheduledTask();
	}
	else {
		//do nothing
	}
}

bool MQClientFactory::updateTopicRouteInfoFromNameServer( std::string& topic )
{
	return updateTopicRouteInfoFromNameServer(topic, false, NULL);
}

void MQClientFactory::updateTopicRouteInfoFromNameServer()
{
	std::set<std::string> topicList;

	// Consumer
	{
		boost::unordered_map<std::string, DefaultMQPullConsumerImpl*>::iterator it = this->consumerTable.begin();
		for(; it != this->consumerTable.end(); it++) {
			DefaultMQPullConsumerImpl* impl = it->second;
			if (impl != NULL) {
				std::set<SubscriptionData*> subList;
				impl->subscriptions(subList);
				if(!subList.empty()) {
					std::set<SubscriptionData*>::iterator it = subList.begin();
					SubscriptionData* subData = NULL;
					for(; it != subList.end(); it++) {
						subData = (*it);
						topicList.insert(subData->getTopic());
					}
				}
			}
		}
	}

	// Producer
	{
		boost::unordered_map<std::string, DefaultMQProducerImpl*>::iterator it = this->producerTable.begin();
		for(; it != this->producerTable.end(); it++) {
			DefaultMQProducerImpl* impl = it->second;
			if(impl != NULL) {
				impl->getPublishTopicList(topicList);
			}
		}
	}

	std::set<std::string>::iterator it = topicList.begin();
	for(; it != topicList.end(); it++) {
		this->updateTopicRouteInfoFromNameServer(*it);
	}
}

bool MQClientFactory::updateTopicRouteInfoFromNameServer( std::string& topic, bool isDefault, DefaultMQProducer* defaultMQProducer )
{
	lockNamesrv.timed_lock(boost::get_system_time() + boost::posix_time::seconds(LockTimeout));
	TopicRouteData* topicRouteData;
	if(isDefault) {
		if(NULL == defaultMQProducer) {
			return false;
		}
		topicRouteData = this->mQClientAPIImpl->getTopicRouteInfoFromNameServer(defaultMQProducer->getCreateTopicKey());
		std::list<QueueData*>::iterator it = topicRouteData->getQueueDatas().begin();
		QueueData* data = NULL;
		for(;it != topicRouteData->getQueueDatas().end(); it++) {
			data = (*it);
			int queueNums = min(defaultMQProducer->getDefaultTopicQueueNums(), data->getReadQueueNums());
			data->setReadQueueNums(queueNums);
			data->setWriteQueueNums(queueNums);
		}
	}
	else {
		topicRouteData = this->mQClientAPIImpl->getTopicRouteInfoFromNameServer(topic);
	}
	if (topicRouteData != NULL) {
		TopicRouteData* old = NULL;
		boost::unordered_map<std::string, TopicRouteData*>::iterator it = this->topicRouteTable.find(topic);
		if (it != this->topicRouteTable.end()) {
			old = it->second;
		}
		bool changed = topicRouteDataIsChange(old, topicRouteData);
		if (!changed) {
			changed = this->isNeedUpdateTopicRouteInfo(topic);
		}
		if (changed) {
			// 更新broker地址信息
			{
				std::list<BrokerData*>::iterator it = topicRouteData->getBrokerDatas().begin();
				for(; it != topicRouteData->getBrokerDatas().end(); it++) {
					BrokerData* brokerdata = *it;
					boost::unordered_map<std::string, std::string>::iterator addrsit = brokerdata->getBrokerAddrs().begin();
					printf("map size %d", brokerdata->getBrokerAddrs().size());
					for(; addrsit != brokerdata->getBrokerAddrs().end(); addrsit++) {
						std::string addr = addrsit->second;
						std::string id = addrsit->first;
						std::cout << std::endl;
						std::cout << id << ",,," << addr <<std::endl;
						(this->brokerAddrTable[brokerdata->getBrokerName()])[id] = addr;
					}
				}
			}

			// 更新发布队列信息
			{
				TopicPublishInfo* publishInfo = topicRouteData2TopicPublishInfo(topic, topicRouteData);
				boost::unordered_map<std::string, DefaultMQProducerImpl*>::iterator it = this->producerTable.begin();
				for(; it != this->producerTable.end(); it++) {
					DefaultMQProducerImpl* impl = it->second;
					impl->updateTopicPublishInfo(topic, publishInfo);
				}
			}

			// 更新订阅队列信息
			{
				std::set<MessageQueue*> subscribeInfo;
				topicRouteData2TopicSubscribeInfo(topic, topicRouteData, subscribeInfo);
				boost::unordered_map<std::string, DefaultMQPullConsumerImpl*>::iterator it = this->consumerTable.begin();
				for(; it != this->consumerTable.end(); it++) {
					DefaultMQPullConsumerImpl* impl = it->second;
					impl->updateTopicSubscribeInfo(topic, subscribeInfo);
				}
			}
			this->topicRouteTable[topic] = topicRouteData;
			lockNamesrv.unlock();
			return true;
		}
	}
	lockNamesrv.unlock();
	return false;
}

TopicPublishInfo* MQClientFactory::topicRouteData2TopicPublishInfo( std::string& topic, TopicRouteData* route )
{
	TopicPublishInfo* info = new TopicPublishInfo();
	// 顺序消息
	if (route->getOrderTopicConf().length() > 0) {
		info->setOrderTopic(true);
	}
	// 非顺序消息
	else {
		list<QueueData*>& qds = route->getQueueDatas();
		// 排序原因：即使没有配置顺序消息模式，默认队列的顺序同配置的一致。
		list<QueueData*>::iterator it = qds.begin();
		for(; it != qds.end(); it++) {
			QueueData* qd = *it;
			for(int i = 0; i < qd->getWriteQueueNums(); i++) {
				MessageQueue* mq = new MessageQueue(topic, qd->getBrokerName(), i);
				info->getMessageQueueList().push_back(mq);
			}		
		}		
		info->setOrderTopic(false);
	}
	return info;
}

void MQClientFactory::topicRouteData2TopicSubscribeInfo( std::string& topic, TopicRouteData* route, std::set<MessageQueue*>& mqList)
{
	std::list<QueueData*>& qds = route->getQueueDatas();
	std::list<QueueData*>::iterator it = qds.begin();
	QueueData* qd = NULL;
	for(; it != qds.end(); it++) {
		qd = (*it);
		for(int i = 0; i < qd->getReadQueueNums(); i++) {
			MessageQueue* mq = new MessageQueue(topic, qd->getBrokerName(), i);
			std::cout << mq->getBrokerName() << std::endl;
			mqList.insert(mq);
		}
	}
}

void MQClientFactory::sendHeartToAllBrokerWithLock()
{
	this->lockHeartbeat.try_lock();
	this->sendHeartbeatToAllBroker();
	this->lockHeartbeat.unlock();

}

void MQClientFactory::sendHeartbeatToAllBroker()
{
	HeartbeatData* heartbeatData = this->prepareHeartbeatData();
	bool producerEmpty = heartbeatData->getProducerDataSet().empty();
	bool consumerEmpty = heartbeatData->getConsumerDataSet().empty();
	if (producerEmpty && consumerEmpty) {
		return;
	}

	boost::unordered_map<std::string, boost::unordered_map<std::string, std::string>>::iterator oneTable;
	oneTable = this->brokerAddrTable.begin();
	boost::unordered_map<std::string, std::string>::iterator id;
	for(; oneTable != this->brokerAddrTable.end(); oneTable++) {
		id = oneTable->second.begin();
		for(; id != oneTable->second.end(); id++) {
			std::string addr = id->second;
			if (!addr.empty()) {
				//说明只有producer， 则不向slave发心跳
				if (consumerEmpty) {
					if (id->first != MixAll::MASTER_ID) {
						continue;
					}
				}
				this->mQClientAPIImpl->sendHeartbeat(addr, heartbeatData,3);
			}
		}
	}
	delete heartbeatData;
}

bool MQClientFactory::registerConsumer( std::string& group, DefaultMQPullConsumerImpl* consumer )
{
	if (group.empty() || consumer == NULL) {
		return false;
	}
	this->consumerTable[group] = consumer;
	// 如果存在就刷新
	return true;
}

bool MQClientFactory::unregisterConsumer( std::string& group )
{
	this->consumerTable.erase(group);
	std::string groupnull = "";
	this->unregisterClientWithLock(groupnull, group);
	return true;
}

void MQClientFactory::unregisterClientWithLock( std::string& producerGroup, std::string& consumerGroup )
{
	this->lockHeartbeat.try_lock();
	this->unregisterClient(producerGroup, consumerGroup);
	this->lockHeartbeat.unlock();
}

void MQClientFactory::unregisterClient( std::string& producerGroup, std::string& consumerGroup )
{

}

bool MQClientFactory::registerProducer( std::string& group, DefaultMQProducerImpl* producer )
{
	if(group.empty() || NULL == producer) {
		return false;
	}
	this->producerTable[group] = producer;
	return true;
}

bool MQClientFactory::unregisterProducer( std::string& group )
{
	this->producerTable.erase(group);
	std::string groupnull = "";
	this->unregisterClientWithLock(group, groupnull);
}

MQClientAPIImpl* MQClientFactory::getMQClientAPIImpl()
{
	return mQClientAPIImpl;
}

bool MQClientFactory::topicRouteDataIsChange( TopicRouteData* oldata, TopicRouteData* nowdata )
{
	if (oldata == NULL || nowdata == NULL) {
		return true;
	}
	return oldata != nowdata;
}

bool MQClientFactory::isNeedUpdateTopicRouteInfo( std::string& topic )
{
	bool result = false;
	// 查看发布队列是否需要更新
	{
		boost::unordered_map<std::string, DefaultMQProducerImpl*>::iterator it = this->producerTable.begin();
		for (; it != this->producerTable.end(); it++) {
			DefaultMQProducerImpl* impl = it->second;
			result = impl->isPublishTopicNeedUpdate(topic);
		}
	}
	// 查看订阅队列是否需要更新
	return result;
}

void MQClientFactory::startScheduledTask()
{
	//创建监控线程
	boost::thread threadScheduledTask(scheduledTask, this);
	threadScheduledTask.detach();
	std::cout << "create scheduledTask" << std::endl;
}

HeartbeatData* MQClientFactory::prepareHeartbeatData()
{
	HeartbeatData* heartbeatData = new HeartbeatData();

	heartbeatData->setClientID(this->clientId);

	// Consumer
	{
		boost::unordered_map<std::string, DefaultMQPullConsumerImpl*>::iterator it = this->consumerTable.begin();
		for(; it != this->consumerTable.end(); it++) {
			DefaultMQPullConsumerImpl* impl = it->second;
			ConsumerData* consumerData = new ConsumerData();
			consumerData->setGroupName(impl->groupName());
			consumerData->setConsumeType(impl->consumeType());
			consumerData->setMessageModel(impl->messageModel());
			consumerData->setConsumeFromWhere(impl->consumeFromWhere());
			std::set<SubscriptionData*> sublist;
			impl->subscriptions(sublist);
			if (!sublist.empty()) {
				std::set<SubscriptionData*>::iterator it = sublist.begin();
				SubscriptionData* subData = NULL;
				for(; it != sublist.end(); it++) {
					subData = *it;
					consumerData->getSubscriptionDataSet().insert(subData);
				}
			}		
			heartbeatData->getConsumerDataSet().insert(consumerData);
		}
	}
	
	// producer
	{
		boost::unordered_map<std::string, DefaultMQProducerImpl*>::iterator it = this->producerTable.begin();
		for(; it != this->producerTable.end(); it++) {
			DefaultMQProducerImpl* impl = it->second;
			ProducerData* producerData = new ProducerData();
			producerData->setGroupName(it->first);
			heartbeatData->getProducerDataSet().insert(producerData);
		}
	}
	
	return heartbeatData;
}

std::string MQClientFactory::findBrokerAddressInPublish( std::string& brokerName )
{
	std::string ret = "";
	boost::unordered_map<std::string, boost::unordered_map<std::string, std::string>>::iterator it;
	it = this->brokerAddrTable.find(brokerName);
	if(it == this->brokerAddrTable.end()) {
		printf("ERROR:: no name in addr when pub");
	}
	else {
		boost::unordered_map<std::string, std::string>::iterator onetable = (it->second).find(MixAll::MASTER_ID);
		ret = onetable->second;
	}
	return ret;
}

void MQClientFactory::shutdown()
{

}

FindBrokerResult* MQClientFactory::findBrokerAddressInSubscribe( std::string& brokerName, long brokerId, bool onlyThisBroker )
{
	std::string brokerAddr;
	boost::unordered_map<std::string, boost::unordered_map<std::string, std::string>>::iterator it;
	it = this->brokerAddrTable.find(brokerName);
	if(it == this->brokerAddrTable.end()) {
		printf("ERROR:: no name in addr when sub");
		return NULL;
	}
	else {
		boost::unordered_map<std::string, std::string>::iterator onetable = (it->second).find(MixAll::MASTER_ID);
		brokerAddr = onetable->second;
		bool slave = false;
		bool found = false;
		return new FindBrokerResult(brokerAddr, slave);
	}
}

void scheduledTask( MQClientFactory* fq )
{
	while(1) {
		fq->updateTopicRouteInfoFromNameServer();
		fq->sendHeartbeatToAllBroker();
#ifdef WIN32
		Sleep(30000);
#else
		sleep(30);
#endif
	}
}