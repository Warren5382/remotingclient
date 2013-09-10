#ifndef COMMANDCUSTOMHEADER_H
#define COMMANDCUSTOMHEADER_H

#include "version.h"
class CommandCustomHeader : public LWPR::Object{
public:
	CommandCustomHeader() {};
	virtual void settoHashMap(boost::unordered_map<std::string, std::string>& thismap) {};
	virtual void getfromHashMap(boost::unordered_map<std::string, std::string>& thismap) {};
};
DECLAREVAR(CommandCustomHeader);

class TestHeader : public CommandCustomHeader{
private:
	std::string messageTitle;
	int count;
public:
	TestHeader() {
	};
	std::string getMessageTitle() {
		return messageTitle;
	}
	void setMessageTitle(std::string messageTitle) {
		this->messageTitle = messageTitle;
	}

	int getCount() {
		return count;
	}
	void setCount(int count) {
		this->count = count;
	}

	void settoHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("messageTitle", messageTitle));
		std::string strcount = boost::lexical_cast <std::string>(count);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("count", strcount));
	}

	void getfromHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		messageTitle = thismap["messageTitle"];
		count = boost::lexical_cast <int>(thismap["count"]);
	}
};
DECLAREVAR(TestHeader);

class GetMaxOffsetResponseHeader : public CommandCustomHeader{
private:
	long offset;
public:
	long getOffset() {
		return offset;
	}

	void setOffset(long offset) {
		this->offset = offset;
	}

	void settoHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("offset", boost::lexical_cast <std::string>(offset)));
	}

	void getfromHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		offset = boost::lexical_cast <long>(thismap["offset"]);
	}
};
DECLAREVAR(GetMaxOffsetResponseHeader);

class GetMaxOffsetRequestHeader : public CommandCustomHeader{
private:
	std::string topic;
	int queueId;

public:

	std::string getTopic() {
		return topic;
	}

	void setTopic(std::string topic) {
		this->topic = topic;
	}
	int getQueueId() {
		return queueId;
	}

	void setQueueId(int queueId) {
		this->queueId = queueId;
	}

	void settoHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("topic", topic));
		std::string strcount = boost::lexical_cast <std::string>(queueId);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("queueId", strcount));
	}

	void getfromHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		topic = thismap["topic"];
		queueId = boost::lexical_cast <int>(thismap["queueId"]);
	}
};
DECLAREVAR(GetMaxOffsetRequestHeader);

class SendMessageRequestHeader : public CommandCustomHeader {
private:
	std::string producerGroup;
	std::string topic;
	std::string defaultTopic;
	int defaultTopicQueueNums;
	int queueId;
	int sysFlag;
	long bornTimestamp;
	int flag;
	std::string properties;
	int reconsumeTimes;
public:
	SendMessageRequestHeader() {
	};
	void settoHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("producerGroup", producerGroup));
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("topic", topic));
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("defaultTopic", defaultTopic));
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("properties", properties));
	
		std::string strcount = boost::lexical_cast <std::string>(defaultTopicQueueNums);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("defaultTopicQueueNums", strcount));
		strcount = boost::lexical_cast <std::string>(queueId);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("queueId", strcount));
		strcount = boost::lexical_cast <std::string>(sysFlag);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("sysFlag", strcount));
		strcount = boost::lexical_cast <std::string>(bornTimestamp);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("bornTimestamp", strcount));
		strcount = boost::lexical_cast <std::string>(flag);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("flag", strcount));
		//strcount = boost::lexical_cast <std::string>(reconsumeTimes);
		//thismap.insert(boost::unordered_map<std::string, std::string>::value_type("reconsumeTimes", strcount));
	};

	void getfromHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		producerGroup = thismap["producerGroup"];
		topic = thismap["topic"];
	};

	std::string& getProducerGroup() {
		return producerGroup;
	};


	void setProducerGroup(std::string producerGroup) {
		this->producerGroup = producerGroup;
	};


	std::string& getTopic() {
		return topic;
	};


	void setTopic(std::string topic) {
		this->topic = topic;
	};


	std::string& getDefaultTopic() {
		return defaultTopic;
	};


	void setDefaultTopic(std::string defaultTopic) {
		this->defaultTopic = defaultTopic;
	};


	int getDefaultTopicQueueNums() {
		return defaultTopicQueueNums;
	};


	void setDefaultTopicQueueNums(int defaultTopicQueueNums) {
		this->defaultTopicQueueNums = defaultTopicQueueNums;
	};


	int getQueueId() {
		return queueId;
	};


	void setQueueId(int queueId) {
		this->queueId = queueId;
	};


	int getSysFlag() {
		return sysFlag;
	};


	void setSysFlag(int sysFlag) {
		this->sysFlag = sysFlag;
	};


	long getBornTimestamp() {
		return bornTimestamp;
	};


	void setBornTimestamp(long bornTimestamp) {
		this->bornTimestamp = bornTimestamp;
	};


	int getFlag() {
		return flag;
	};


	void setFlag(int flag) {
		this->flag = flag;
	};


	std::string getProperties() {
		return properties;
	};


	void setProperties(std::string properties) {
		this->properties = properties;
	};


	int getReconsumeTimes() {
		return reconsumeTimes;
	};


	void setReconsumeTimes(int reconsumeTimes) {
		this->reconsumeTimes = reconsumeTimes;
	};
};
DECLAREVAR(SendMessageRequestHeader);

class PullMessageRequestHeader : public CommandCustomHeader {
private:
	std::string consumerGroup;
	std::string topic;
	int queueId;
	long queueOffset;
	int maxMsgNums;
	int sysFlag;
	long commitOffset;
	long suspendTimeoutMillis;
	std::string subscription;
	long subVersion;
public:
	PullMessageRequestHeader() {

	};

	std::string getConsumerGroup() {
		return consumerGroup;
	};


	void setConsumerGroup(std::string consumerGroup) {
		this->consumerGroup = consumerGroup;
	};


	std::string getTopic() {
		return topic;
	};


	void setTopic(std::string topic) {
		this->topic = topic;
	};


	int getQueueId() {
		return queueId;
	};


	void setQueueId(int queueId) {
		this->queueId = queueId;
	};


	long getQueueOffset() {
		return queueOffset;
	};


	void setQueueOffset(long queueOffset) {
		this->queueOffset = queueOffset;
	};


	int getMaxMsgNums() {
		return maxMsgNums;
	};


	void setMaxMsgNums(int maxMsgNums) {
		this->maxMsgNums = maxMsgNums;
	};


	int getSysFlag() {
		return sysFlag;
	};


	void setSysFlag(int sysFlag) {
		this->sysFlag = sysFlag;
	};


	long getCommitOffset() {
		return commitOffset;
	};


	void setCommitOffset(long commitOffset) {
		this->commitOffset = commitOffset;
	};


	long getSuspendTimeoutMillis() {
		return suspendTimeoutMillis;
	};


	void setSuspendTimeoutMillis(long suspendTimeoutMillis) {
		this->suspendTimeoutMillis = suspendTimeoutMillis;
	};


	std::string getSubscription() {
		return subscription;
	};


	void setSubscription(std::string subscription) {
		this->subscription = subscription;
	};


	long getSubVersion() {
		return subVersion;
	};


	void setSubVersion(long subVersion) {
		this->subVersion = subVersion;
	};

	void settoHashMap(boost::unordered_map<std::string, std::string>& thismap) {


		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("consumerGroup", consumerGroup));
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("topic", topic));
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("subscription", subscription));

		std::string strcount = boost::lexical_cast <std::string>(queueId);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("queueId", strcount));
		strcount = boost::lexical_cast <std::string>(queueOffset);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("queueOffset", strcount));
		strcount = boost::lexical_cast <std::string>(maxMsgNums);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("maxMsgNums", strcount));
		strcount = boost::lexical_cast <std::string>(sysFlag);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("sysFlag", strcount));
		strcount = boost::lexical_cast <std::string>(commitOffset);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("commitOffset", strcount));
		strcount = boost::lexical_cast <std::string>(suspendTimeoutMillis);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("suspendTimeoutMillis", strcount));
		strcount = boost::lexical_cast <std::string>(subVersion);
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("subVersion", strcount));
	};
};
DECLAREVAR(PullMessageRequestHeader);

class PullMessageResponseHeader : public CommandCustomHeader{
private:
	long suggestWhichBrokerId;
	long nextBeginOffset;
	long minOffset;
	long maxOffset;

public:
	PullMessageResponseHeader() {

	};

	long getNextBeginOffset() {
		return nextBeginOffset;
	};


	void setNextBeginOffset(long nextBeginOffset) {
		this->nextBeginOffset = nextBeginOffset;
	};


	long getMinOffset() {
		return minOffset;
	};


	void setMinOffset(long minOffset) {
		this->minOffset = minOffset;
	};


	long getMaxOffset() {
		return maxOffset;
	};

	void setMaxOffset(long maxOffset) {
		this->maxOffset = maxOffset;
	};


	long getSuggestWhichBrokerId() {
		return suggestWhichBrokerId;
	};


	void setSuggestWhichBrokerId(long suggestWhichBrokerId) {
		this->suggestWhichBrokerId = suggestWhichBrokerId;
	};

	void getfromHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		suggestWhichBrokerId = boost::lexical_cast <long>(thismap["suggestWhichBrokerId"]);
		nextBeginOffset = boost::lexical_cast <long>(thismap["nextBeginOffset"]);
		minOffset = boost::lexical_cast <long>(thismap["minOffset"]);
		maxOffset = boost::lexical_cast <long>(thismap["maxOffset"]);
	};
};
DECLAREVAR(PullMessageResponseHeader);

class GetRouteInfoRequestHeader : public CommandCustomHeader {
private:
	std::string topic;
public:
	std::string& getTopic() {
		return topic;
	};
	void setTopic(std::string& topic) {
		this->topic = topic;
	};
	void settoHashMap(boost::unordered_map<std::string, std::string>& thismap) {
		thismap.insert(boost::unordered_map<std::string, std::string>::value_type("topic", topic));
	};
};
DECLAREVAR(GetRouteInfoRequestHeader);

#endif