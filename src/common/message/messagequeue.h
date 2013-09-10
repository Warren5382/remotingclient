#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

class MessageQueue : public CJsonObjectBase {
private:
	long long serialVersionUID;
	std::string topic;
	std::string brokerName;
	int queueId;
public:
	MessageQueue() {};
	MessageQueue(std::string topic, std::string brokerName, int queueId) {
		this->topic = topic;
		this->brokerName = brokerName;
		this->queueId = queueId;
		serialVersionUID = 6191200464116433425L;
	};
	virtual void SetPropertys() {
		SetProperty("serialVersionUID", asInt64, &serialVersionUID);
		SetProperty("topic", asString, &topic);
		SetProperty("brokerName", asString, &brokerName);
		SetProperty("queueId", asInt, &queueId);
	};

	std::string& getBrokerName() {
		return brokerName;
	};

	std::string& getTopic() {
		return topic;
	};

	int getQueueId() {
		return queueId;
	};
};

#endif
