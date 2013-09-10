#ifndef MESSAGE_H
#define MESSAGE_H
#include "version.h"
/**
* 消息，Producer与Consumer使用
* 
* @author shijia.wxr<vintage.wang@gmail.com>
* @since 2013-7-18
*/
class Message : public LWPR::Object{
private:
	long long serialVersionUID;
	/**
	* 消息主题
	*/
	std::string topic;
	/**
	* 消息标志，系统不做干预，完全由应用决定如何使用
	*/
	int flag;
	/**
	* 消息属性，系统有保留属性，应用也可以自定义属性
	*/
	boost::unordered_map<std::string, std::string> properties;
	/**
	* 消息体
	*/
	std::string body;
public:
	/**
	* 消息关键词，多个Key用KEY_SEPARATOR隔开（查询消息使用）
	*/
	std::string PROPERTY_KEYS;
	/**
	* 消息标签，只支持设置一个Tag（服务端消息过滤使用）
	*/
	std::string PROPERTY_TAGS;
	/**
	* 是否等待服务器将消息存储完毕再返回（可能是等待刷盘完成或者等待同步复制到其他服务器）
	*/
	std::string PROPERTY_WAIT_STORE_MSG_OK;
	/**
	* 消息延时投递时间级别，0表示不延时，大于0表示特定延时级别（具体级别在服务器端定义）
	*/
	std::string PROPERTY_DELAY_TIME_LEVEL;

	/**
	* 内部使用
	*/
	std::string PROPERTY_RETRY_TOPIC ;
	std::string PROPERTY_REAL_TOPIC ;
	std::string PROPERTY_REAL_QUEUE_ID;
	std::string PROPERTY_TRANSACTION_PREPARED;
	std::string PROPERTY_PRODUCER_GROUP;
	std::string PROPERTY_MIN_OFFSET;
	std::string PROPERTY_MAX_OFFSET;

	std::string KEY_SEPARATOR;


	Message() {
	}


	Message(std::string topic, std::string body) {
		constructmsg(topic, "", "", 0, body, true);
	}


	Message(std::string topic, std::string tags, std::string body) {
		constructmsg(topic, tags, "", 0, body, true);
	}


	Message(std::string topic, std::string tags, std::string keys, std::string body) {
		constructmsg(topic, tags, keys, 0, body, true);
	}


	void constructmsg(std::string topic, std::string tags, std::string keys, int flag, std::string body, bool waitStoreMsgOK) {
		serialVersionUID = 8445773977080406428L;
		PROPERTY_KEYS = "KEYS";
		PROPERTY_TAGS = "TAGS";
		PROPERTY_WAIT_STORE_MSG_OK = "WAIT";
		PROPERTY_DELAY_TIME_LEVEL = "DELAY";
		PROPERTY_RETRY_TOPIC = "RETRY_TOPIC";
		PROPERTY_REAL_TOPIC = "REAL_TOPIC";
		PROPERTY_REAL_QUEUE_ID = "REAL_QID";
		PROPERTY_TRANSACTION_PREPARED = "TRAN_MSG";
		PROPERTY_PRODUCER_GROUP = "PGROUP";
		PROPERTY_MIN_OFFSET = "MIN_OFFSET";
		PROPERTY_MAX_OFFSET = "MAX_OFFSET";
		KEY_SEPARATOR = " ";
		this->topic = topic;
		this->flag = flag;
		this->body = body;

		if (tags.length() > 0)
			this->setTags(tags);

		if (keys.length() > 0)
			this->setKeys(keys);

		this->setWaitStoreMsgOK(waitStoreMsgOK);
	}


	void clearProperty(std::string& name) {
		this->properties.erase(name);
	}


	void putProperty(std::string name, std::string value) {
		this->properties[name] = value;
	}


	std::string& getProperty(std::string& name) {
		return this->properties[name];
	}


	std::string& getTopic() {
		return topic;
	}


	void setTopic(std::string& topic) {
		this->topic = topic;
	}


	std::string& getTags() {
		return this->getProperty(PROPERTY_TAGS);
	}


	void setTags(std::string& tags) {
		this->putProperty(PROPERTY_TAGS, tags);
	}


	std::string& getKeys() {
		return this->getProperty(PROPERTY_KEYS);
	}


	void setKeys(std::string& keys) {
		this->putProperty(PROPERTY_KEYS, keys);
	}



	int getDelayTimeLevel() {
		std::string& t = this->getProperty(PROPERTY_DELAY_TIME_LEVEL);
		if (!t.empty()) {
			return 1;
		}
		return 0;
	}


	void setDelayTimeLevel(int level) {
		this->putProperty(PROPERTY_DELAY_TIME_LEVEL, "1");
	}


	bool isWaitStoreMsgOK() {
		std::string result = this->getProperty(PROPERTY_WAIT_STORE_MSG_OK);
		if (result.empty())
			return true;

		if(result == "TRUE") {
			return true;
		}
		else {
			return false;
		}
	}


	void setWaitStoreMsgOK(boolean waitStoreMsgOK) {
		this->putProperty(PROPERTY_WAIT_STORE_MSG_OK, "TRUE");
	}


	int getFlag() {
		return flag;
	}


	void setFlag(int flag) {
		this->flag = flag;
	}


	std::string& getBody() {
		return body;
	}


	void setBody(std::string& body) {
		this->body = body;
	}


	boost::unordered_map<std::string, std::string>& getProperties() {
		return properties;
	}


	void setProperties(boost::unordered_map<std::string, std::string>& properties) {
		this->properties = properties;
	}
};

DECLAREVAR(Message);

#endif