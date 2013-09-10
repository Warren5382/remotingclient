#ifndef MIXALL_H
#define MIXALL_H
#include "version.h"
class MixAll {
public:
	static std::string ROCKETMQ_HOME_ENV ;
	static std::string ROCKETMQ_HOME_PROPERTY ;

	static std::string NAMESRV_ADDR_ENV;
	static std::string NAMESRV_ADDR_PROPERTY;

	static std::string WS_DOMAIN_NAME;

	static std::string WS_ADDR;
	static std::string DEFAULT_TOPIC;
	static std::string DEFAULT_PRODUCER_GROUP;
	static std::string DEFAULT_CONSUMER_GROUP;
	static std::string CLIENT_INNER_PRODUCER_GROUP;
	static std::string SELF_TEST_TOPIC;

//	static std::string DEFAULT_CHARSET;
	static std::string MASTER_ID;

	// Ϊÿ��Consumer Group����һ��Ĭ�ϵ�Topic��ǰ׺ + GroupName���������洦��ʧ����Ҫ���Ե���Ϣ
	static std::string RETRY_GROUP_TOPIC_PREFIX;
	// Ϊÿ��Consumer Group����һ��Ĭ�ϵ�Topic��ǰ׺ + GroupName�������������Զ�ζ�ʧ�ܣ��������������Ե���Ϣ
	static std::string DLQ_GROUP_TOPIC_PREFIX;
};

class ConsumeType {
public:
	static std::string CONSUME_ACTIVELY;
	static std::string CONSUME_PASSIVELY;
};

class MessageModel {
public:
	/**
	* �㲥ģ��
	*/
	static std::string BROADCASTING;
	/**
	* ��Ⱥģ��
	*/
	static std::string CLUSTERING;
};

class ConsumeFromWhere {
public:
	/**
	* ÿ�����������ϴμ�¼��λ�㿪ʼ���ѣ�����ǵ�һ������������λ�㿪ʼ���ѣ���������������ʹ��
	*/
	static std::string CONSUME_FROM_LAST_OFFSET;
	/**
	* ÿ�����������ϴμ�¼��λ�㿪ʼ���ѣ�����ǵ�һ�����������Сλ�㿪ʼ���ѣ��������ʱʹ��<br>
	* ���ϻ����������������Ҫ��ˣ�������Ч
	*/
	static std::string CONSUME_FROM_LAST_OFFSET_AND_FROM_MIN_WHEN_BOOT_FIRST;
	/**
	* ÿ������������Сλ�㿪ʼ���ѣ��������ʱʹ��<br>
	* ���ϻ����������������Ҫ��ˣ�������Ч
	*/
	static std::string CONSUME_FROM_MIN_OFFSET;
	/**
	* ÿ�������������λ�㿪ʼ���ѣ��������ʱʹ��
	*/
	static std::string CONSUME_FROM_MAX_OFFSET;
};

class SendStatus {
public:
	static std::string SEND_OK;
	static std::string FLUSH_DISK_TIMEOUT;
	static std::string FLUSH_SLAVE_TIMEOUT;
	static std::string SLAVE_NOT_AVAILABLE;
};

class ServiceState {
public:
	static std::string CREATE_JUST;
	static std::string RUNNING;
	static std::string SHUTDOWN_ALREADY;
};

class CommunicationMode {
public:
	static std::string SYNC;
	static std::string ASYNC;
	static std::string ONEWAY;
};

#endif