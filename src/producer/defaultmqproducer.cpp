#include "DefaultMQProducer.h"
#include "..\impl\producer\DefaultMQProducerImpl.h"
#include "common\mixall.h"
#include "sendresult.h"
#include "common\message\message.h"
#include "common\message\messageext.h"

DefaultMQProducer::DefaultMQProducer(): producerGroup(MixAll::DEFAULT_PRODUCER_GROUP), createTopicKey(MixAll::DEFAULT_TOPIC), defaultTopicQueueNums(4), sendMsgTimeout(10), compressMsgBodyOverHowmuch(1024*4), retryAnotherBrokerWhenNotStoreOK(false), maxMessageSize(1024*128) {
	defaultMQProducerImpl = new DefaultMQProducerImpl(this);
}

DefaultMQProducer::DefaultMQProducer(std::string& producerGroup): producerGroup(MixAll::DEFAULT_PRODUCER_GROUP), createTopicKey(MixAll::DEFAULT_TOPIC), defaultTopicQueueNums(4), sendMsgTimeout(10), compressMsgBodyOverHowmuch(1024*4), retryAnotherBrokerWhenNotStoreOK(false), maxMessageSize(1024*128) {
	this->producerGroup = producerGroup;
	defaultMQProducerImpl = new DefaultMQProducerImpl(this);
}



void DefaultMQProducer::start() {
	this->defaultMQProducerImpl->start();
}

void DefaultMQProducer::shutdown() {
	this->defaultMQProducerImpl->shutdown();
}

std::list<MessageQueue*>& DefaultMQProducer::fetchPublishMessageQueues(std::string& topic) {
	return this->defaultMQProducerImpl->fetchPublishMessageQueues(topic);
}

SendResult_ptr DefaultMQProducer::send(Message_ptr msg) {
	return this->defaultMQProducerImpl->send(msg);
}

void DefaultMQProducer::sendOneway(Message_ptr msg) {
	this->defaultMQProducerImpl->sendOneway(msg);
}

SendResult_ptr DefaultMQProducer::send(Message_ptr msg, MessageQueue* mq) {
	return this->defaultMQProducerImpl->send(msg, mq);
}

void DefaultMQProducer::sendOneway(Message_ptr msg, MessageQueue* mq) {
	this->defaultMQProducerImpl->sendOneway(msg, mq);
}

void DefaultMQProducer::createTopic(std::string& key, std::string& newTopic, int queueNum)  {
	this->defaultMQProducerImpl->createTopic(key, newTopic, queueNum);
}

MessageExt DefaultMQProducer::viewMessage(std::string& msgId)  {
	return this->defaultMQProducerImpl->viewMessage(msgId);
}

std::string& DefaultMQProducer::getProducerGroup() {
	return producerGroup;
}

void DefaultMQProducer::setProducerGroup(std::string& producerGroup) {
	this->producerGroup = producerGroup;
}

std::string& DefaultMQProducer::getCreateTopicKey() {
	return createTopicKey;
}

void DefaultMQProducer::setCreateTopicKey(std::string& createTopicKey) {
	this->createTopicKey = createTopicKey;
}

int DefaultMQProducer::getSendMsgTimeout() {
	return sendMsgTimeout;
}

void DefaultMQProducer::setSendMsgTimeout(int sendMsgTimeout) {
	this->sendMsgTimeout = sendMsgTimeout;
}


int DefaultMQProducer::getCompressMsgBodyOverHowmuch() {
	return compressMsgBodyOverHowmuch;
}


void DefaultMQProducer::setCompressMsgBodyOverHowmuch(int compressMsgBodyOverHowmuch) {
	this->compressMsgBodyOverHowmuch = compressMsgBodyOverHowmuch;
}


DefaultMQProducerImpl* DefaultMQProducer::getDefaultMQProducerImpl() {
	return defaultMQProducerImpl;
}


bool DefaultMQProducer::isRetryAnotherBrokerWhenNotStoreOK() {
	return retryAnotherBrokerWhenNotStoreOK;
}


void DefaultMQProducer::setRetryAnotherBrokerWhenNotStoreOK(bool retryAnotherBrokerWhenNotStoreOK) {
	this->retryAnotherBrokerWhenNotStoreOK = retryAnotherBrokerWhenNotStoreOK;
}


int DefaultMQProducer::getMaxMessageSize() {
	return maxMessageSize;
}


void DefaultMQProducer::setMaxMessageSize(int maxMessageSize) {
	this->maxMessageSize = maxMessageSize;
}


int DefaultMQProducer::getDefaultTopicQueueNums() {
	return defaultTopicQueueNums;
}


void DefaultMQProducer::setDefaultTopicQueueNums(int defaultTopicQueueNums) {
	this->defaultTopicQueueNums = defaultTopicQueueNums;
}

DefaultMQProducer::~DefaultMQProducer()
{
	delete defaultMQProducerImpl;

}