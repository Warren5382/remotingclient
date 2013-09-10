#ifndef MQCONSUMERINNER_H
#define MQCONSUMERINNER_H

#include "version.h"
#include "common/message/MessageQueue.h"

class MQConsumerInner{
public:
	virtual std::string groupName();
	virtual std::string messageModel();
	virtual std::string consumerType();
	virtual std::string consumeFromWhere();
	//virtual std::set<SubscriptionData>& subscriptions() = 0;
	virtual void doRebalance();
	virtual void persistConsumerOffset();
	virtual void updateTopicSubscribeInfo(std::string& topic, std::set<MessageQueue*>& info);
	virtual bool isSubscribeTopicNeedUpdate(std::string& topic);
};

#endif