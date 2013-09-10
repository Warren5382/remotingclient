#ifndef HEARTBEATDATA_H
#define HEARTBEATDATA_H
#include <set>
#include "subscriptiondata.h"
#include "version.h"

class ProducerData : public CJsonObjectBase{
public:
	ProducerData(){
		SetPropertys();
	};
	~ProducerData(){};
	std::string& getGroupName() {
		return groupName;
	};
	void setGroupName(std::string groupName) {
		this->groupName = groupName;
	};
private:
	std::string groupName;
protected:
	virtual void SetPropertys() {
		SetProperty("groupName", asString, &groupName);
	};
};

class ConsumerData : public CJsonObjectBase{
public:
	ConsumerData(){
		SetPropertys();
	};
	~ConsumerData(){};
	std::string getGroupName() {
		return groupName;
	};
	void setGroupName(std::string groupName) {
		this->groupName = groupName;
	};
	std::string& getConsumeType() {
		return consumeType;
	};
	void setConsumeType(std::string consumeType) {
		this->consumeType = consumeType;
	};
	std::string& getMessageModel() {
		return messageModel;
	};
	void setMessageModel(std::string messageModel) {
		this->messageModel = messageModel;
	};
	std::string& getConsumeFromWhere() {
		return consumeFromWhere;
	};
	void setConsumeFromWhere(std::string consumeFromWhere) {
		this->consumeFromWhere = consumeFromWhere;
	};
	std::set<SubscriptionData*>& getSubscriptionDataSet() {
		return subscriptionDataSet;
	};
	void setSubscriptionDataSet(std::set<SubscriptionData*>& subscriptionDataSet) {
	}
private:
	std::string groupName;
	std::string consumeType;
	std::string messageModel;
	std::string consumeFromWhere;
	std::set<SubscriptionData*> subscriptionDataSet;
protected:
	CJsonObjectBase* GenerateJsonObjForDeSerialize(const string& propertyName) {
		if("subscriptionData" == propertyName) {
			return new SubscriptionData();
		}
		return NULL;
	};
	virtual void SetPropertys() {
		SetProperty("groupName", asString, &groupName);
		SetProperty("consumeType", asString, &consumeType);
		SetProperty("messageModel", asString, &messageModel);
		SetProperty("consumeFromWhere", asString, &consumeFromWhere);
		SetProperty("subscriptionDataSet", asSetArray, &subscriptionDataSet, asJsonObj);
	};
};

class HeartbeatData : public CJsonObjectBase {
public:
	HeartbeatData(){
		SetPropertys();
	};
	~HeartbeatData(){};
	std::string getClientID() {
		return clientID;
	};
	void setClientID(std::string clientID) {
		this->clientID = clientID;
	};	
	std::set<ProducerData*>& getProducerDataSet() {
		return producerDataSet;
	};
	std::set<ConsumerData*>& getConsumerDataSet() {
		return consumerDataSet;
	};
private:
	std::string clientID;
	std::set<ProducerData*> producerDataSet;
	std::set<ConsumerData*> consumerDataSet;
protected:
	CJsonObjectBase* GenerateJsonObjForDeSerialize(const string& propertyName) {
		if("ProducerData" == propertyName) {
			return new ProducerData();
		}
		else if("ConsumerData" == propertyName) {
			return new ConsumerData();
		}
		else {
			return NULL;
		}
	};
	virtual void SetPropertys() {
		SetProperty("clientID", asString, &clientID);
		SetProperty("producerDataSet", asSetArray, &producerDataSet, asJsonObj);
		SetProperty("consumerDataSet", asSetArray, &consumerDataSet, asJsonObj);
	};
};
#endif