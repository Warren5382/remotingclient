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

	// 为每个Consumer Group建立一个默认的Topic，前缀 + GroupName，用来保存处理失败需要重试的消息
	static std::string RETRY_GROUP_TOPIC_PREFIX;
	// 为每个Consumer Group建立一个默认的Topic，前缀 + GroupName，用来保存重试多次都失败，接下来不再重试的消息
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
	* 广播模型
	*/
	static std::string BROADCASTING;
	/**
	* 集群模型
	*/
	static std::string CLUSTERING;
};

class ConsumeFromWhere {
public:
	/**
	* 每次启动都从上次记录的位点开始消费，如果是第一次启动则从最大位点开始消费，建议在生产环境使用
	*/
	static std::string CONSUME_FROM_LAST_OFFSET;
	/**
	* 每次启动都从上次记录的位点开始消费，如果是第一次启动则从最小位点开始消费，建议测试时使用<br>
	* 线上环境此配置项可能需要审核，否则无效
	*/
	static std::string CONSUME_FROM_LAST_OFFSET_AND_FROM_MIN_WHEN_BOOT_FIRST;
	/**
	* 每次启动都从最小位点开始消费，建议测试时使用<br>
	* 线上环境此配置项可能需要审核，否则无效
	*/
	static std::string CONSUME_FROM_MIN_OFFSET;
	/**
	* 每次启动都从最大位点开始消费，建议测试时使用
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