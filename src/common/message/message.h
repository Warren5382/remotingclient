#ifndef MESSAGE_H
#define MESSAGE_H
#include "version.h"
/**
* ��Ϣ��Producer��Consumerʹ��
* 
* @author shijia.wxr<vintage.wang@gmail.com>
* @since 2013-7-18
*/
class Message : public LWPR::Object{
private:
	long long serialVersionUID;
	/**
	* ��Ϣ����
	*/
	std::string topic;
	/**
	* ��Ϣ��־��ϵͳ������Ԥ����ȫ��Ӧ�þ������ʹ��
	*/
	int flag;
	/**
	* ��Ϣ���ԣ�ϵͳ�б������ԣ�Ӧ��Ҳ�����Զ�������
	*/
	boost::unordered_map<std::string, std::string> properties;
	/**
	* ��Ϣ��
	*/
	std::string body;
public:
	/**
	* ��Ϣ�ؼ��ʣ����Key��KEY_SEPARATOR��������ѯ��Ϣʹ�ã�
	*/
	std::string PROPERTY_KEYS;
	/**
	* ��Ϣ��ǩ��ֻ֧������һ��Tag���������Ϣ����ʹ�ã�
	*/
	std::string PROPERTY_TAGS;
	/**
	* �Ƿ�ȴ�����������Ϣ�洢����ٷ��أ������ǵȴ�ˢ����ɻ��ߵȴ�ͬ�����Ƶ�������������
	*/
	std::string PROPERTY_WAIT_STORE_MSG_OK;
	/**
	* ��Ϣ��ʱͶ��ʱ�伶��0��ʾ����ʱ������0��ʾ�ض���ʱ���𣨾��弶���ڷ������˶��壩
	*/
	std::string PROPERTY_DELAY_TIME_LEVEL;

	/**
	* �ڲ�ʹ��
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