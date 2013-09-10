#include "DefaultMQPullConsumer.h"
#include "common\mixall.h"
#include "impl\consumer\defaultmqpullconsumerimpl.h"
#include "common\message\messageext.h"
#include "queryresult.h"
#include "PullResult.h"

DefaultMQPullConsumer::DefaultMQPullConsumer(): brokerSuspendMaxTime(20), consumerPullTimeout(10), consumerTimeoutWhenSuspend(30), consumerGroup(MixAll::DEFAULT_CONSUMER_GROUP)
,messageModel(MessageModel::BROADCASTING){
	this->defaultMQPullConsumerImpl = new DefaultMQPullConsumerImpl(this);
}


DefaultMQPullConsumer::DefaultMQPullConsumer(std::string& consumerGroup): brokerSuspendMaxTime(20), consumerPullTimeout(10), consumerTimeoutWhenSuspend(30), consumerGroup(MixAll::DEFAULT_CONSUMER_GROUP)
,messageModel(MessageModel::BROADCASTING) {
	this->consumerGroup = consumerGroup;
	this->defaultMQPullConsumerImpl = new DefaultMQPullConsumerImpl(this);
}


void DefaultMQPullConsumer::createTopic(std::string& key, std::string& newTopic, int queueNum) {
	this->defaultMQPullConsumerImpl->createTopic(key, newTopic, queueNum);
}


long DefaultMQPullConsumer::searchOffset(MessageQueue* mq, long timestamp) {
	return this->defaultMQPullConsumerImpl->searchOffset(mq, timestamp);
}


long DefaultMQPullConsumer::maxOffset(MessageQueue* mq) {
	return this->defaultMQPullConsumerImpl->maxOffset(mq);
}


long DefaultMQPullConsumer::minOffset(MessageQueue* mq) {
	return this->defaultMQPullConsumerImpl->minOffset(mq);
}


long DefaultMQPullConsumer::earliestMsgStoreTime(MessageQueue* mq) {
	return this->defaultMQPullConsumerImpl->earliestMsgStoreTime(mq);
}


MessageExt DefaultMQPullConsumer::viewMessage(std::string& msgId) {
	return this->defaultMQPullConsumerImpl->viewMessage(msgId);
}

QueryResult DefaultMQPullConsumer::queryMessage(std::string&  topic, std::string&  key, int maxNum, long begin, long end) {
	return this->defaultMQPullConsumerImpl->queryMessage(topic, key, maxNum, begin, end);
}

long DefaultMQPullConsumer::getBrokerSuspendMaxTime() {
	return brokerSuspendMaxTime;
}

void DefaultMQPullConsumer::setBrokerSuspendMaxTime(long brokerSuspendMaxTime) {
	this->brokerSuspendMaxTime = brokerSuspendMaxTime;
}

std::string& DefaultMQPullConsumer::getConsumerGroup() {
	return consumerGroup;
}

void DefaultMQPullConsumer::setConsumerGroup(std::string& consumerGroup) {
	this->consumerGroup = consumerGroup;
}
long DefaultMQPullConsumer::getConsumerPullTimeout() {
	return consumerPullTimeout;
}

void DefaultMQPullConsumer::setConsumerPullTimeout(long consumerPullTimeout) {
	this->consumerPullTimeout = consumerPullTimeout;
}


long DefaultMQPullConsumer::getConsumerTimeoutWhenSuspend() {
	return consumerTimeoutWhenSuspend;
}


void DefaultMQPullConsumer::setConsumerTimeoutWhenSuspend(long consumerTimeoutWhenSuspend) {
	this->consumerTimeoutWhenSuspend = consumerTimeoutWhenSuspend;
}


std::string DefaultMQPullConsumer::getMessageModel() {
	return messageModel;
}


void DefaultMQPullConsumer::setMessageModel(std::string messageModel) {
	this->messageModel = messageModel;
}


set<std::string>& DefaultMQPullConsumer::getRegisterTopics() {
	return registerTopics;
}


void DefaultMQPullConsumer::setRegisterTopics(set<std::string>& registerTopics) {
	this->registerTopics = registerTopics;
}

void DefaultMQPullConsumer::sendMessageBack(MessageExt msg, int delayLevel){
	this->defaultMQPullConsumerImpl->sendMessageBack(msg, delayLevel);
}

void DefaultMQPullConsumer::fetchSubscribeMessageQueues(std::string& topic, std::set<MessageQueue*>& mqs){
	this->defaultMQPullConsumerImpl->fetchSubscribeMessageQueues(topic, mqs);
}

void DefaultMQPullConsumer::start() {
	this->defaultMQPullConsumerImpl->start();
}

void DefaultMQPullConsumer::shutdown() {
	this->defaultMQPullConsumerImpl->shutdown();
}

PullResult_ptr DefaultMQPullConsumer::pull(MessageQueue* mq, std::string& subExpression, long offset, int maxNums){
	return this->defaultMQPullConsumerImpl->pull(mq, subExpression, offset, maxNums);
}

void DefaultMQPullConsumer::pull(MessageQueue* mq, std::string& subExpression, long offset, int maxNums,
		  bool pullCallback) {
			  this->defaultMQPullConsumerImpl->pull(mq, subExpression, offset, maxNums, pullCallback);
}


PullResult_ptr DefaultMQPullConsumer::pullBlockIfNotFound(MessageQueue* mq, std::string& subExpression, long offset, int maxNums){
	return this->defaultMQPullConsumerImpl->pullBlockIfNotFound(mq, subExpression, offset, maxNums);
}

void DefaultMQPullConsumer::pullBlockIfNotFound(MessageQueue* mq, std::string& subExpression, long offset, int maxNums, bool pullCallback) {
	this->defaultMQPullConsumerImpl->pullBlockIfNotFound(mq, subExpression, offset, maxNums, pullCallback);
}

void DefaultMQPullConsumer::updateConsumeOffset(MessageQueue* mq, long offset) {
	//this->defaultMQPullConsumerImpl->updateConsumeOffset(mq, offset);
}

long DefaultMQPullConsumer::fetchConsumeOffset(MessageQueue* mq, bool fromStore) {
	return this->defaultMQPullConsumerImpl->fetchConsumeOffset(mq, fromStore);
}

set<MessageQueue*>& DefaultMQPullConsumer::fetchMessageQueuesInBalance(std::string& topic) {
	return this->defaultMQPullConsumerImpl->fetchMessageQueuesInBalance(topic);
}

void DefaultMQPullConsumer::registerMessageQueueListener( std::string& topic, MessageQueueListener* listener )
{
	this->registerTopics.insert(topic);
	if(listener != NULL) {
		this->messageQueueListener = listener;
	}
}