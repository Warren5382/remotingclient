#include "DefaultMQPullConsumerImpl.h"
#include "consumer\defaultmqpullconsumer.h"
#include "common\Message\MessageQueue.h"
#include "consumer\PullResult.h"
#include "queryresult.h"
#include "..\Factory\MQClientFactory.h"
#include "impl\mqclientmanager.h"
#include "protocol\subscriptiondata.h"
#include "common\mixall.h"
#include "mqclientapiimpl.h"
#include "common\filter\filterapi.h"
#include "pullapiwrapper.h"
#include "common\sysflag\pullsysflag.h"

DefaultMQPullConsumerImpl::DefaultMQPullConsumerImpl( DefaultMQPullConsumer* defaultMQPullConsumer ) : serviceState(ServiceState::CREATE_JUST)
{
	this->defaultMQPullConsumer = defaultMQPullConsumer;
}

//需要admin，暂缓开发
void DefaultMQPullConsumerImpl::createTopic( std::string& topic, std::string& newTopic, int queueNum )
{
	this->makeSureStateOK();
}

void DefaultMQPullConsumerImpl::makeSureStateOK()
{
	if(this->serviceState != "RUNNING") {
		//error

	}

}

long DefaultMQPullConsumerImpl::fetchConsumeOffset(MessageQueue* mq, bool fromStore)
{
	this->makeSureStateOK();
	//return this->offsetStore.readOffset(mq, fromStore);
	return 1;
}

std::set<MessageQueue*>& DefaultMQPullConsumerImpl::fetchMessageQueuesInBalance( std::string& topic )
{
	this->makeSureStateOK();
	if(topic.empty()) {
		//topic is null
	}
	std::set<MessageQueue*> q;
	return q;
}

//做不了

std::string DefaultMQPullConsumerImpl::groupName()
{
	return this->defaultMQPullConsumer->getConsumerGroup();
}

std::string DefaultMQPullConsumerImpl::messageModel()
{
	return this->defaultMQPullConsumer->getMessageModel();
}

std::string DefaultMQPullConsumerImpl::consumeType()
{
	return ConsumeType::CONSUME_ACTIVELY;
}

std::string DefaultMQPullConsumerImpl::consumeFromWhere()
{
	return ConsumeFromWhere::CONSUME_FROM_LAST_OFFSET;
}

void DefaultMQPullConsumerImpl::subscriptions(std::set<SubscriptionData*>& result)
{
	std::set<std::string> topics = this->getDefaultMQPullConsumer()->getRegisterTopics();
	if (!topics.empty()) {
		std::set<std::string>::iterator it = topics.begin();
		for(; it != topics.end(); it++) {
			std::cout << "topics " << *it << std::endl;
			SubscriptionData* ms = new SubscriptionData((*it), SubscriptionData::SUB_ALL);
			result.insert(ms);
		}
	}
}

void DefaultMQPullConsumerImpl::doRebalance()
{

}

void DefaultMQPullConsumerImpl::updateTopicSubscribeInfo( std::string& topic, std::set<MessageQueue*>& info )
{

}

bool DefaultMQPullConsumerImpl::isSubscribeTopicNeedUpdate( std::string& topic )
{

}

long DefaultMQPullConsumerImpl::maxOffset( MessageQueue* mq )
{

}

long DefaultMQPullConsumerImpl::minOffset( MessageQueue* mq )
{

}

PullResult_ptr DefaultMQPullConsumerImpl::pull( MessageQueue* mq, std::string subExpression, long offset, int maxNums )
{

}

void DefaultMQPullConsumerImpl::pull( MessageQueue* mq, std::string subExpression, long offset, int maxNums, bool PullCallback )
{

}
PullResult_ptr DefaultMQPullConsumerImpl::pullBlockIfNotFound( MessageQueue* mq, std::string subExpression, long offset, int maxNums )
{
	return this->pullSyncImpl(mq, subExpression, offset, maxNums, true);
}

void DefaultMQPullConsumerImpl::pullBlockIfNotFound( MessageQueue* mq, std::string subExpression, long offset, int maxNums, bool PullCallback )
{

}
// 不用实现
QueryResult DefaultMQPullConsumerImpl::queryMessage( std::string& topic, std::string key, int maxNum, long begin, long end )
{
	this->makeSureStateOK();

}

long DefaultMQPullConsumerImpl::searchOffset( MessageQueue* mq, long timestamp )
{

}


void DefaultMQPullConsumerImpl::shutdown()
{
	if(this->serviceState == ServiceState::RUNNING) {
		this->persistConsumerOffset();
		this->mQClientFactory->unregisterConsumer(this->defaultMQPullConsumer->getConsumerGroup());
		this->mQClientFactory->shutdown();
		this->serviceState = ServiceState::SHUTDOWN_ALREADY;
	}
}

void DefaultMQPullConsumerImpl::start()
{
	if(this->serviceState == ServiceState::CREATE_JUST) {
		this->copySubscription();
		this->serviceState = ServiceState::RUNNING;
		this->mQClientFactory = MQClientManager::getInstance()->getAndCreateMQClientFactory();
		//初始化 Rebalance变量
		this->pullAPIWrapper = new PullAPIWrapper(this->mQClientFactory, this->defaultMQPullConsumer->getConsumerGroup());

		bool registerOK = mQClientFactory->registerConsumer(this->defaultMQPullConsumer->getConsumerGroup(), this);
		if(!registerOK) {
			this->serviceState = ServiceState::CREATE_JUST;
		}
		this->mQClientFactory->start();
	}
}

void DefaultMQPullConsumerImpl::copySubscription()
{
	// 复制用户初始设置的订阅关系

}

void DefaultMQPullConsumerImpl::updateConsumeoffset(MessageQueue* mq, long offset)
{
	this->makeSureStateOK();

}

MessageExt DefaultMQPullConsumerImpl::viewMessage( std::string& msgId )
{
	//暂时不做

}

DefaultMQPullConsumer* DefaultMQPullConsumerImpl::getDefaultMQPullConsumer()
{
	return defaultMQPullConsumer;
}


PullResult_ptr DefaultMQPullConsumerImpl::pullSyncImpl( MessageQueue* mq, std::string subExpression, long offset, int maxNums, bool block )
{
	this->makeSureStateOK();
	PullResult_var pullResult;
	if(NULL == mq) {
		return NULL;
	}
	if(offset < 0) {
		return NULL;
	}
	if(maxNums <= 0) {
		return NULL;
	}

	int sysFlag = PullSysFlag::buildSysFlag(false, block, true);

	SubscriptionData* subscriptionData = FilterAPI::buildSubscriptionData(mq->getTopic(), subExpression);
	
	pullResult = this->pullAPIWrapper->pullKernelImpl(
		mq, // 1
		subscriptionData->getSubString(), // 2
		0L, // 3
		offset, // 4
		maxNums, // 5
		sysFlag, // 6
		0, // 7
		this->defaultMQPullConsumer->getBrokerSuspendMaxTime(), // 8
		3, // 9
		CommunicationMode::SYNC, // 10
		false// 11
		);
	this->pullAPIWrapper->processPullResult(mq, pullResult, subscriptionData);
	delete subscriptionData;
	if(pullResult != NULL) {
		return PullResult_var::Duplicate(pullResult);
	}
	else {
		return NULL;
	}
	
};

void DefaultMQPullConsumerImpl::persistConsumerOffset()
{

}

void DefaultMQPullConsumerImpl::sendMessageBack( MessageExt msg, int delayLevel )
{

}

long DefaultMQPullConsumerImpl::earliestMsgStoreTime( MessageQueue* mq )
{
	return 1;
}

std::list<MessageQueue*>& DefaultMQPullConsumerImpl::fetchPublishMessageQueues( std::string& topic )
{
	std::list<MessageQueue*> mq;
	return mq;
}

void DefaultMQPullConsumerImpl::fetchSubscribeMessageQueues( std::string& topic, std::set<MessageQueue*>& mqList)
{
	TopicRouteData* topicRouteData = this->mQClientFactory->getMQClientAPIImpl()->getTopicRouteInfoFromNameServer(topic);
	if (topicRouteData != NULL) {
		MQClientFactory::topicRouteData2TopicSubscribeInfo(topic, topicRouteData, mqList);
		if (!mqList.empty()) {
		}
		else {
			printf("the MqList is empty!\n");
		}
	}
}