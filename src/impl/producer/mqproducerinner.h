#ifndef MQPRODUCERINNER_H
#define MQPRODUCERINNER_H
#include "version.h"
#include "../factory/TopicPublishInfo.h"
class MQProducerInner {
 public:
	 virtual std::set<std::string>& getPublishTopicList();

	 virtual bool isPublishTopicNeedUpdate(std::string& topic);

	 /*
    virtual TransactionCheckListener checkListener()=0;


    virtual void checkTransactionState(//
            std::string& addr, //
            MessageExt msg, //
            CheckTransactionStateRequestHeader checkRequestHeader)=0;
*/

    virtual void updateTopicPublishInfo(std::string& topic, TopicPublishInfo* info);
};
#endif