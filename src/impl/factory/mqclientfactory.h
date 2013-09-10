#ifndef MQCLIENTFACTORY_H
#define MQCLIENTFACTORY_H
#include "version.h"

class TopicRouteData;
class DefaultMQProducer;
class MQClientAPIImpl;
class TopicRouteData;
class TopicPublishInfo;
class MessageQueue;
class HeartbeatData;
class FindBrokerResult;
class DefaultMQProducerImpl;
class DefaultMQPullConsumerImpl;

class MQClientFactory {
private:
	std::string serviceState;
	int factoryIndex;
	std::string clientId;
	long bootTimestamp;

	boost::unordered_map<std::string, DefaultMQProducerImpl*> producerTable;
	boost::unordered_map<std::string, DefaultMQPullConsumerImpl*> consumerTable;

	MQClientAPIImpl* mQClientAPIImpl;

	boost::unordered_map<std::string, TopicRouteData*> topicRouteTable;
	boost::timed_mutex lockNamesrv;
	boost::timed_mutex lockHeartbeat;
	int LockTimeout;

	//存储Broker Name 与Broker Address的对应关系
	boost::unordered_map<std::string, boost::unordered_map<std::string, std::string>> brokerAddrTable;
	//监听一个UDP
	//拉消息服务
	//PullMessageService pullMessageService;
	//RebalanceService rebalanceService;
	DefaultMQProducer* defaultMQProducer;

public:
	MQClientFactory(int factoryIndex, std::string clientId);
	~MQClientFactory();
	void startScheduledTask();
	void start();
	void shutdown();
	MQClientAPIImpl* getMQClientAPIImpl();
	HeartbeatData* prepareHeartbeatData();
	void sendHeartbeatToAllBroker();
	void sendHeartToAllBrokerWithLock();
	bool registerConsumer(std::string& group, DefaultMQPullConsumerImpl* consumer);
	bool unregisterConsumer(std::string& group);
	void unregisterClientWithLock(std::string& producerGroup, std::string& consumerGroup);
	void unregisterClient(std::string& producerGroup, std::string& consumerGroup);
	bool registerProducer(std::string& group, DefaultMQProducerImpl* producer);
	bool unregisterProducer(std::string& group);
	DefaultMQProducerImpl* selectProducer(std::string& group);
	DefaultMQPullConsumerImpl* selectConsumer(std::string& group);
	//发布消息过程中，寻找Broker地址，一定是找Master
	std::string findBrokerAddressInPublish(std::string& brokerName);
	//订阅消息过程中，寻找Broker地址，取Master还是Slave有参数决定
	FindBrokerResult* findBrokerAddressInSubscribe(std::string& brokerName, long brokerId, bool onlyThisBroker);
	std::string findBrokerAddrByTopic(std::string& topic);
	static TopicPublishInfo* topicRouteData2TopicPublishInfo(std::string& topic, TopicRouteData* route);
	static void topicRouteData2TopicSubscribeInfo(std::string& topic, TopicRouteData* route, std::set<MessageQueue*>& mqList);
	void updateTopicRouteInfoFromNameServer();
	bool updateTopicRouteInfoFromNameServer(std::string& topic);
	bool updateTopicRouteInfoFromNameServer(std::string& topic, bool isDefault, DefaultMQProducer* defaultMQProducer);
	bool isNeedUpdateTopicRouteInfo(std::string& topic);
	bool topicRouteDataIsChange(TopicRouteData* oldata, TopicRouteData* nowdata);
};

void scheduledTask(MQClientFactory* fq);
#endif