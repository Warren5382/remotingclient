#ifndef DEFAULTMQPULLCONSUMER_H
#define DEFAULTMQPULLCONSUMER_H
#include "version.h"
#include "PullResult.h"

class DefaultMQPullConsumerImpl;
class MessageExt;
class QueryResult;
class MessageQueue;
class MessageQueueListener;
class OffsetStore;



class DefaultMQPullConsumer {
	public:
		DefaultMQPullConsumerImpl* defaultMQPullConsumerImpl;
		std::string consumerGroup;
	private:
		long brokerSuspendMaxTime;
		long consumerTimeoutWhenSuspend;
		long consumerPullTimeout;
		std::string messageModel;
		MessageQueueListener* messageQueueListener;
		//OffsetStore offsetStore;
		std::set<std::string> registerTopics;
		//AllocateMessageQueueStrategy allocateMessageQueueStrategy = new AllocateMessageQueueAveragely();
	public:
		DefaultMQPullConsumer();
		DefaultMQPullConsumer(std::string& consumerGroup);
		void createTopic(std::string& key, std::string& newTopic, int queueNum);
		long searchOffset(MessageQueue* mq, long timestamp);
		long maxOffset(MessageQueue* mq);
		long minOffset(MessageQueue* mq);
		long earliestMsgStoreTime(MessageQueue* mq);
		MessageExt viewMessage(std::string& msgId);
		QueryResult queryMessage(std::string&  topic, std::string&  key, int maxNum, long start, long over);
		//AllocateMessageQueueStrategy getAllocateMessageQueueStrategy();
		//void setAllocateMessageQueueStrategy(AllocateMessageQueueStrategy allocateMessageQueueStrategy);
		long getBrokerSuspendMaxTime();
		void setBrokerSuspendMaxTime(long brokerSuspendMaxTime);
		std::string& getConsumerGroup();
		void setConsumerGroup(std::string& consumerGroup);
		long getConsumerPullTimeout();
		void setConsumerPullTimeout(long consumerPullTimeout);
		long getConsumerTimeoutWhenSuspend();
		void setConsumerTimeoutWhenSuspend(long consumerTimeoutWhenSuspend);
		std::string getMessageModel();
		void setMessageModel(std::string messageModel);
		MessageQueueListener* getMessageQueueListener();
		void setMessageQueueListener(MessageQueueListener* messageQueueListener);
		std::set<std::string>& getRegisterTopics();
		void setRegisterTopics(std::set<std::string>& registerTopics);
		void sendMessageBack(MessageExt msg, int delayLevel);
		void fetchSubscribeMessageQueues(std::string& topic, std::set<MessageQueue*>& mqs);
		void start();
		void shutdown();
		void registerMessageQueueListener(std::string& topic, MessageQueueListener* listener);
		PullResult_ptr pull(MessageQueue* mq, std::string& subExpression, long offset, int maxNums);
		void pull(MessageQueue* mq, std::string& subExpression, long offset, int maxNums, bool pullCallback);
		PullResult_ptr pullBlockIfNotFound(MessageQueue* mq, std::string& subExpression, long offset, int maxNums);
		void pullBlockIfNotFound(MessageQueue* mq, std::string& subExpression, long offset, int maxNums, bool pullCallback);
		void updateConsumeOffset(MessageQueue* mq, long offset);
		long fetchConsumeOffset(MessageQueue* mq, bool fromStore);
		std::set<MessageQueue*>& fetchMessageQueuesInBalance(std::string& topic);
		OffsetStore getOffsetStore();
		void setOffsetStore(OffsetStore offsetStore);
};

#endif
