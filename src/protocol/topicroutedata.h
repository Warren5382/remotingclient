#ifndef TOPICROUTEDATA_H
#define TOPICROUTEDATA_H
#include "version.h"

class BrokerData : public CJsonObjectBase{
private:
	std::string brokerName;
	boost::unordered_map<std::string, std::string> brokerAddrs;
protected:
	virtual void SetPropertys() {
		SetProperty("brokerName", asString, &brokerName);
		SetProperty("brokerAddrs", asHashMap, &brokerAddrs);
	};
public:
	BrokerData() {
		SetPropertys();
	};
	std::string getBrokerName() {
		return brokerName;
	};
	void setBrokerName(std::string brokerName) {
		this->brokerName = brokerName;
	};
	boost::unordered_map<std::string, std::string>& getBrokerAddrs() {
		return brokerAddrs;
	};
	void setBrokerAddrs() {};
	bool equals() {return true;};
};

class QueueData : public CJsonObjectBase{
private:
	std::string brokerName;
	int readQueueNums;
	int writeQueueNums;
	int perm;
protected:
	virtual void SetPropertys() {
		SetProperty("brokerName", asString, &brokerName);
		SetProperty("readQueueNums", asInt, &readQueueNums);
		SetProperty("writeQueueNums", asInt, &writeQueueNums);
		SetProperty("perm", asInt, &perm);
	};
public:
	QueueData() {
		SetPropertys();
	};
	std::string& getBrokerName() {
		return brokerName;
	};
	void setBrokerName(std::string brokerName) {
		this->brokerName = brokerName;
	};
	void setReadQueueNums(int readQueueNums) {
		this->readQueueNums = readQueueNums;
	};
	int getReadQueueNums() {
		return readQueueNums;
	};
	void setWriteQueueNums(int writeQueueNums) {
		this->writeQueueNums = writeQueueNums;
	};
	int getWriteQueueNums() {
		return writeQueueNums;
	};
};
class TopicRouteData : public CJsonObjectBase{
private:
	std::string orderTopicConf;
	list<QueueData*> queueDatas;
	list<BrokerData*> brokerDatas;

protected:
	virtual CJsonObjectBase* GenerateJsonObjForDeSerialize(const string& propertyName) {
		if("queueDatas" == propertyName) {
			return new QueueData();
		}
		else if("brokerDatas" == propertyName) {
			return new BrokerData();
		}
		else return NULL;
	};
	virtual void SetPropertys() {
		//SetProperty("orderTopicConf", asString, &orderTopicConf);
		SetProperty("queueDatas", asListArray, &queueDatas, asJsonObj);
		SetProperty("brokerDatas", asListArray, &brokerDatas, asJsonObj);
	};

public:
	TopicRouteData() {
		SetPropertys();
	};
	list<QueueData*>& getQueueDatas() {
		return queueDatas;
	};
	void setQueueDatas() {};

	list<BrokerData*>& getBrokerDatas() {
		return brokerDatas;
	};
	void setBrokerDatas() {};

	std::string& getOrderTopicConf() {
		return orderTopicConf;
	};
	void setOrderTopicConf(std::string& orderTopicConf) {
		this->orderTopicConf = orderTopicConf;
	};
};
#endif