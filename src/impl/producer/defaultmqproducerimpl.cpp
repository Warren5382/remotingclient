#include "DefaultMQProducerImpl.h"
#include "unitest\testproimpl.h"
#include "producer\defaultmqproducer.h"
#include "impl\mqclientmanager.h"
#include "common\mixall.h"
#include "common\message\messageext.h"
#include "producer\SendResult.h"
#include "mqclientapiimpl.h"
#include "commandheader\commandcustomheader.h"
#include "..\Factory\TopicPublishInfo.h"


DefaultMQProducerImpl::DefaultMQProducerImpl(DefaultMQProducer* defaultMQProducer):serviceState(ServiceState::CREATE_JUST) {
	this->defaultMQProducer = defaultMQProducer;
}

void DefaultMQProducerImpl::start() {
	this->start(true);
}

void DefaultMQProducerImpl::start(bool startFactory)  {
	if(this->serviceState == ServiceState::CREATE_JUST) {
		this->serviceState = ServiceState::RUNNING;
		this->mQClientFactory = MQClientManager::getInstance()->getAndCreateMQClientFactory();

		bool registerOK = mQClientFactory->registerProducer(defaultMQProducer->getProducerGroup(), this);
		if (!registerOK) {
			this->serviceState = ServiceState::CREATE_JUST;
			printf("The producer group has been created before, specify another name please.\n");
			return (void)0;
		}

		// 默认Topic注册
		TopicPublishInfo* topicPublishInfo = new TopicPublishInfo();
		this->topicPublishInfoTable[defaultMQProducer->getCreateTopicKey()] = topicPublishInfo; 

		if (startFactory) {
			this->mQClientFactory->start();
		}
	}
	this->mQClientFactory->sendHeartToAllBrokerWithLock();
}


void DefaultMQProducerImpl::checkConfig()  {
	/*
	if (null == this->defaultMQProducer.getProducerGroup()) {
		throw new MQClientException("producerGroup is null", null);
	}

	if (this->defaultMQProducer.getProducerGroup().equals(MixAll.DEFAULT_PRODUCER_GROUP)) {
		throw new MQClientException("producerGroup can not equal " + MixAll.DEFAULT_PRODUCER_GROUP
				+ ", please specify another one.", null);
	}*/
}


void DefaultMQProducerImpl::shutdown() {
	this->shutdown(true);
}


void DefaultMQProducerImpl::shutdown(bool shutdownFactory) {
	/*
	if(this->serviceState == ServiceState::RUNNING) {
		this->mQClientFactory->unregisterProducer(this->defaultMQProducer->getProducerGroup());
		if (shutdownFactory) {
			this->mQClientFactory->shutdown();
			this->serviceState = "SHUTDOWN_ALREADY";
		}
	}
	*/
}

bool DefaultMQProducerImpl::getPublishTopicList(std::set<std::string>& topicList) {
	boost::unordered_map<std::string, TopicPublishInfo*>::iterator it = this->topicPublishInfoTable.begin();
	for(; it != this->topicPublishInfoTable.end(); it++) {
		topicList.insert(it->first);
	}
	return true;
}


bool DefaultMQProducerImpl::isPublishTopicNeedUpdate(std::string& topic) {
	boost::unordered_map<std::string, TopicPublishInfo*>::iterator it;
	it = this->topicPublishInfoTable.find(topic);
	if(it == this->topicPublishInfoTable.end()) {
		return false;
	}
	return true;
}

void DefaultMQProducerImpl::updateTopicPublishInfo(std::string& topic, TopicPublishInfo* info) {
	if (info != NULL && !topic.empty()) {
		this->topicPublishInfoTable[topic] = info;
	}
}


void DefaultMQProducerImpl::createTopic(std::string& key, std::string& newTopic, int queueNum)  {
	this->makeSureStateOK();
	//this->mQClientFactory.getMQAdminImpl().createTopic(key, newTopic, queueNum);
}

void DefaultMQProducerImpl::makeSureStateOK()  {
	if (this->serviceState != "RUNNING") {
		printf("state is not in running\n");
	}
}


std::list<MessageQueue*>& DefaultMQProducerImpl::fetchPublishMessageQueues(std::string& topic)  {
	this->makeSureStateOK();
}

MessageExt DefaultMQProducerImpl::viewMessage(std::string& msgId){
	this->makeSureStateOK();
}

SendResult_ptr DefaultMQProducerImpl::sendDefaultImpl(Message_ptr msg, std::string& communicationMode, bool sendCallback) {
	long beginTimestamp = 0;
	long endTimestamp = beginTimestamp;
	TopicPublishInfo* topicPublishInfo = this->tryToFindTopicPublishInfo(msg->getTopic());
	if (topicPublishInfo->ok()) {
		MessageQueue* mq = NULL;
		SendResult_var sendResult = new SendResult();
		for (int times = 0; times < 3; times++) {
			std::string lastBrokerName;
			if(mq != NULL) {
				lastBrokerName = mq->getBrokerName();
			}
			MessageQueue* tmpmq = topicPublishInfo->selectOneMessageQueue(lastBrokerName);
			if (tmpmq != NULL) {
				mq = tmpmq;
				sendResult = this->sendKernelImpl(msg, mq, communicationMode, sendCallback);
				endTimestamp = 12;
				if(communicationMode == CommunicationMode::SYNC) {
					if (sendResult->getSendStatus() != SendStatus::SEND_OK) {
						/*
						if (this->defaultMQProducer->isRetryAnotherBrokerWhenNotStoreOK()) {
							continue;
						}
						*/
					}
					return SendResult_var::Duplicate(sendResult);
				}
			}
			else {
				break;
			}
		} 
		return SendResult_var::Duplicate(sendResult);
	}
	return NULL;
}


/**
 * 尝试寻找Topic路由信息，如果没有则到Name Server上找，再没有，则取默认Topic
 */
TopicPublishInfo* DefaultMQProducerImpl::tryToFindTopicPublishInfo(std::string& topic) {
	boost::unordered_map<std::string, TopicPublishInfo*>::iterator it;
	it = this->topicPublishInfoTable.find(topic);
	if( it == this->topicPublishInfoTable.end()) {
		TopicPublishInfo* topicPublishInfo = new TopicPublishInfo();
		this->topicPublishInfoTable[topic] = topicPublishInfo;
	}
	
	it = this->topicPublishInfoTable.find(topic);

	if(!it->second->ok()) {
		this->mQClientFactory->updateTopicRouteInfoFromNameServer(topic);
		this->mQClientFactory->updateTopicRouteInfoFromNameServer(topic, true, this->defaultMQProducer);
	}


	if (it != this->topicPublishInfoTable.end() && it->second->ok()) {
		return it->second;
	}

	return this->topicPublishInfoTable[this->defaultMQProducer->getCreateTopicKey()];
}


SendResult_ptr DefaultMQProducerImpl::sendKernelImpl(Message_ptr msg, MessageQueue* mq, std::string& communicationMode, bool sendCallback) {
	std::string brokerAddr = this->mQClientFactory->findBrokerAddressInPublish(mq->getBrokerName());
	if (brokerAddr.empty()) {
		// TODO 此处可能对Name Server压力过大，需要调优
		this->mQClientFactory->updateTopicRouteInfoFromNameServer(mq->getTopic());
		this->mQClientFactory->updateTopicRouteInfoFromNameServer(mq->getTopic(), true, this->defaultMQProducer);
		brokerAddr = this->mQClientFactory->findBrokerAddressInPublish(mq->getBrokerName());
	}

	if (!brokerAddr.empty()) {
		std::string prevBody = msg->getBody();
		int sysFlag = 0;

		SendMessageRequestHeader_var requestHeader = new SendMessageRequestHeader();
		requestHeader->setProducerGroup(this->defaultMQProducer->getProducerGroup());
		requestHeader->setTopic(msg->getTopic());
		requestHeader->setDefaultTopic(this->defaultMQProducer->getCreateTopicKey());
		requestHeader->setDefaultTopicQueueNums(this->defaultMQProducer->getDefaultTopicQueueNums());
		requestHeader->setQueueId(mq->getQueueId());
		requestHeader->setSysFlag(sysFlag);
		requestHeader->setBornTimestamp(112233);
		requestHeader->setFlag(msg->getFlag());

		SendResult_var sendResult = new SendResult();
		this->mQClientFactory->getMQClientAPIImpl()->sendMessage(brokerAddr, mq->getBrokerName(), SendMessageRequestHeader_var::Duplicate(requestHeader), 3);
		return SendResult_var::Duplicate(sendResult);
	}
	return NULL;
}

bool DefaultMQProducerImpl::tryToCompressMessage(Message_ptr msg) {
	/*
	   byte[] body = msg.getBody();
	   if (body != null) {
	   if (body.length >= this->defaultMQProducer.getCompressMsgBodyOverHowmuch()) {
	   byte[] data = UtilALl.compress(body, 9);
	   if (data != null) {
	   msg.setBody(data);
	   return true;
	   }
	   }*/

	return true;
}


void DefaultMQProducerImpl::checkMessage(Message_ptr msg)  {
	/*
	   if (null == msg) {
	   throw new MQClientException("the message is null", null);
	   }
	// topic
	if (null == msg.getTopic()) {
	throw new MQClientException("the message topic is null", null);
	}
	// body
	if (null == msg.getBody()) {
	throw new MQClientException("the message body is null", null);
	}

	if (0 == msg.getBody().length) {
	throw new MQClientException("the message body length is zero", null);
	}

	if (msg.getBody().length > this->defaultMQProducer.getMaxMessageSize()) {
	throw new MQClientException("the message body size over max value, MAX: "
	+ this->defaultMQProducer.getMaxMessageSize(), null);
	}
	*/
}


/**
 * DEFAULT ONEWAY -------------------------------------------------------
 */
void DefaultMQProducerImpl::sendOneway(Message_ptr msg) {
	this->makeSureStateOK();

	this->checkMessage(msg);

	this->sendDefaultImpl(msg, CommunicationMode::ONEWAY, false);
}


/**
 * KERNEL SYNC -------------------------------------------------------
 */
SendResult_ptr DefaultMQProducerImpl::send(Message_ptr msg, MessageQueue* mq) {
	this->makeSureStateOK();

	this->checkMessage(msg);

	return this->sendKernelImpl(msg, mq, CommunicationMode::SYNC, false);
}

/**
 * KERNEL ONEWAY -------------------------------------------------------
 */
void DefaultMQProducerImpl::sendOneway(Message_ptr msg, MessageQueue* mq) {
	this->makeSureStateOK();

	this->checkMessage(msg);

	this->sendKernelImpl(msg, mq, CommunicationMode::ONEWAY, false);
}

/**
 * DEFAULT SYNC -------------------------------------------------------
 */
SendResult_ptr DefaultMQProducerImpl::send(Message_ptr msg) {
	this->makeSureStateOK();

	this->checkMessage(msg);

	return this->sendDefaultImpl(msg, CommunicationMode::SYNC, false);
}
