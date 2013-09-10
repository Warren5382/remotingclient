#ifndef DEFAULTMQPULLCONSUMERIMPL_H
#define DEFAULTMQPULLCONSUMERIMPL_H
#include "version.h"
#include "consumer\PullResult.h"

class DefaultMQPullConsumer;
class MQClientFactory;
class OffsetStore;
class MessageQueue;
class SubscriptionData;
class MessageExt;
class QueryResult;
class PullAPIWrapper;

class DefaultMQPullConsumerImpl {
private:
	DefaultMQPullConsumer* defaultMQPullConsumer;
	std::string serviceState;
	MQClientFactory* mQClientFactory;
	PullAPIWrapper* pullAPIWrapper;
	//rebalance
	//OffsetStore offsetStore;
	//RebalanceImpl* rebalanceImpl;
	void makeSureStateOK();
public:
	DefaultMQPullConsumerImpl(DefaultMQPullConsumer* defaultMQPullConsumer);
	void createTopic(std::string& topic, std::string& newTopic, int queueNum);
	long fetchConsumeOffset(MessageQueue* mq, bool fromStore);
	std::set<MessageQueue*>& fetchMessageQueuesInBalance(std::string& topic);
	
	// 需要admin，暂时做不了
	std::list<MessageQueue*>& fetchPublishMessageQueues(std::string& topic);
	void fetchSubscribeMessageQueues( std::string& topic, std::set<MessageQueue*>& mqList);
	long earliestMsgStoreTime(MessageQueue* mq);

	std::string groupName();
	std::string messageModel();
	std::string consumeType() ;
	std::string consumeFromWhere();
	void subscriptions(std::set<SubscriptionData*>& result);
	void doRebalance();
	void persistConsumerOffset();
	void updateTopicSubscribeInfo(std::string& topic, std::set<MessageQueue*>& info);
	bool isSubscribeTopicNeedUpdate(std::string& topic);
	long maxOffset(MessageQueue* mq);
	long minOffset(MessageQueue* mq);
	void pull(MessageQueue* mq, std::string subExpression, long offset, int maxNums, bool PullCallback);
	PullResult_ptr pull(MessageQueue* mq, std::string subExpression, long offset, int maxNums);
	PullResult_ptr pullSyncImpl(MessageQueue* mq, std::string subExpression, long offset, int maxNums, bool block);
	PullResult_ptr pullBlockIfNotFound(MessageQueue* mq, std::string subExpression, long offset, int maxNums);
	void pullBlockIfNotFound(MessageQueue* mq, std::string subExpression, long offset, int maxNums, bool PullCallback);
	QueryResult queryMessage(std::string& topic, std::string key, int maxNum, long begin, long end);
	long searchOffset(MessageQueue* mq, long timestamp);
	void sendMessageBack(MessageExt msg, int delayLevel);
	void shutdown();
	void start();
	void copySubscription();
	void updateConsumeoffset();
	void updateConsumeoffset(MessageQueue* mq, long offset);
	MessageExt viewMessage(std::string& msgId);
	DefaultMQPullConsumer* getDefaultMQPullConsumer();
	//OffsetStore getOffsetStore();
	void setOffsetStore(OffsetStore offsetStore);
};
#endif