#ifndef DEFAULTPRODUCERIMPL_H
#define DEFAULTPRODUCERIMPL_H

#include "version.h"
#include "producer\SendResult.h"
#include "common\message\message.h"
class DefaultMQProducer;
class TopicPublishInfo;
class MQClientFactory;
class MessageQueue;
class MessageExt;

/**
* 生产者默认实现
* 
*/
class DefaultMQProducerImpl {
private:
	 DefaultMQProducer* defaultMQProducer;
	 boost::unordered_map<std::string, TopicPublishInfo*> topicPublishInfoTable;
	/**
	* 事务相关
	*/
	std::string serviceState;
	MQClientFactory* mQClientFactory;
public:
	DefaultMQProducerImpl(DefaultMQProducer* defaultMQProducer);
	void start();
	void start(bool startFactory);
	void shutdown();
	bool getPublishTopicList(std::set<std::string>&);
	bool isPublishTopicNeedUpdate(std::string& topic);
	void updateTopicPublishInfo(std::string& topic, TopicPublishInfo* info);
	void createTopic(std::string& key, std::string& newTopic, int queueNum);
	std::list<MessageQueue*>& fetchPublishMessageQueues(std::string& topic);
	MessageExt viewMessage(std::string& msgId);
	SendResult_ptr sendDefaultImpl(Message_ptr msg, std::string& communicationMode, bool sendCallback);
	void sendOneway(Message_ptr msg);
	SendResult_ptr send(Message_ptr msg, MessageQueue* mq);
	void sendOneway(Message_ptr msg, MessageQueue* mq);
	SendResult_ptr send(Message_ptr msg);

private:
	void checkConfig();
	void shutdown(bool shutdownFactory);
	void makeSureStateOK();
	TopicPublishInfo* tryToFindTopicPublishInfo(std::string& topic);
	SendResult_ptr sendKernelImpl(Message_ptr msg, MessageQueue* mq, std::string& communicationMode, bool sendCallback);
	bool tryToCompressMessage(Message_ptr msg);
	void checkMessage(Message_ptr msg);
};

#endif
