#ifndef MQPROTOS_H
#define MQPROTOS_H

enum MQRequestCode {
	// Broker 发送消息
	SEND_MESSAGE = 10,
	// Broker 订阅消息
	PULL_MESSAGE = 11,
	// Broker 查询消息
	QUERY_MESSAGE = 12,
	// Broker 查询Broker Offset
	QUERY_BROKER_OFFSET = 13,
	// Broker 查询Consumer Offset
	QUERY_CONSUMER_OFFSET = 14,
	// Broker 更新Consumer Offset
	UPDATE_CONSUMER_OFFSET = 15,
	// Broker 更新或者增加一个Topic
	UPDATE_AND_CREATE_TOPIC = 17,
	// Broker 删除一个Topic，包含数据与配置
	DELETE_TOPIC = 19,
	// Broker 获取所有Topic的配置（Slave和Namesrv都会向Master请求此配置）
	GET_ALL_TOPIC_CONFIG = 21,
	// Broker 获取所有Topic配置（Slave和Namesrv都会向Master请求此配置）
	GET_TOPIC_CONFIG_LIST = 22,
	// Broker 获取所有Topic名称列表
	GET_TOPIC_NAME_LIST = 23,
	// Broker Slave获取Master所有Consumer消费进度
	PULL_ALL_CONSUMER_OFFSET = 24,
	// Broker 更新Broker上的配置
	UPDATE_BROKER_CONFIG = 25,
	// Broker 获取Broker上的配置
	GET_BROKER_CONFIG = 26,
	// Broker 触发Broker删除文件
	TRIGGER_DELETE_FILES = 27,
	// Broker 获取Broker运行时信息
	GET_BROKER_RUNTIME_INFO = 28,
	// Broker 根据时间查询队列的Offset
	SEARCH_OFFSET_BY_TIMESTAMP = 29,
	// Broker 查询队列最大Offset
	GET_MAX_OFFSET = 30,
	// Broker 查询队列最小Offset
	GET_MIN_OFFSET = 31,
	// Broker 查询队列最早消息对应时间
	GET_EARLIEST_MSG_STORETIME = 32,
	// Broker 根据消息ID来查询消息
	VIEW_MESSAGE_BY_ID = 33,
	// Broker Client向Client发送心跳，并注册自身
	HEART_BEAT = 34,
	// Broker Client注销
	UNREGISTER_CLIENT = 35,
	// Broker Consumer将处理不了的消息发回服务器
	CONSUMER_SEND_MSG_BACK = 36,
	// Broker Commit或者Rollback事务
	END_TRANSACTION = 37,
	// Broker 获取ConsumerId列表通过GroupName
	GET_CONSUMER_LIST_BY_GROUP = 38,
	// Broker 主动向Producer回查事务状态
	CHECK_TRANSACTION_STATE = 39,
	// Broker Broker通知Consumer列表变化
	NOTIFY_CONSUMER_IDS_CHANGED = 40,
	// Broker Consumer向Master锁定队列
	LOCK_BATCH_MQ = 41,
	// Broker Consumer向Master解锁队列
	UNLOCK_BATCH_MQ = 42,
	// Broker 获取所有Consumer Offset
	GET_ALL_CONSUMER_OFFSET = 43,
	// Broker 获取所有定时进度
	GET_ALL_DELAY_OFFSET = 45,
	// Namesrv 向Namesrv追加KV配置
	PUT_KV_CONFIG = 100,
	// Namesrv 从Namesrv获取KV配置
	GET_KV_CONFIG = 101,
	// Namesrv 从Namesrv获取KV配置
	DELETE_KV_CONFIG = 102,
	// Namesrv 注册一个Broker，数据都是持久化的，如果存在则覆盖配置
	REGISTER_BROKER = 103,
	// Namesrv 卸载一个Broker，数据都是持久化的
	UNREGISTER_BROKER = 104,
	// Namesrv 根据Topic获取Broker Name、队列数(包含读队列与写队列)
	GET_ROUTEINTO_BY_TOPIC = 105,
	// Namesrv 获取注册到Name Server的所有Broker集群信息
	GET_BROKER_CLUSTER_INFO = 106,

	// Broker 更新或者增加一个订阅组
	UPDATE_AND_CREATE_SUBSCRIPTIONGROUP = 200,
	GET_ALL_SUBSCRIPTIONGROUP_CONFIG = 201
};

enum MQResponseCode {
	// Broker 刷盘超时
	FLUSH_DISK_TIMEOUT = 10,
	// Broker 同步双写，Slave不可用
	SLAVE_NOT_AVAILABLE = 11,
	// Broker 同步双写，等待Slave应答超时
	FLUSH_SLAVE_TIMEOUT = 12,
	// Broker 消息非法
	MESSAGE_ILLEGAL = 13,
	// Broker, Namesrv 服务不可用，可能是正在关闭或者权限问题
	SERVICE_NOT_AVAILABLE = 14,
	// Broker, Namesrv 版本号不支持
	VERSION_NOT_SUPPORTED = 15,
	// Broker, Namesrv 无权限执行此操作，可能是发、收、或者其他操作
	NO_PERMISSION = 16,
	// Broker, Topic不存在
	TOPIC_NOT_EXIST = 17,
	// Broker, Topic已经存在，创建Topic
	TOPIC_EXIST_ALREADY = 18,
	// Broker 拉消息未找到（请求的Offset等于最大Offset，最大Offset无对应消息）
	PULL_NOT_FOUND = 19,
	// Broker 可能被过滤，或者误通知等
	PULL_RETRY_IMMEDIATELY = 20,
	// Broker 拉消息请求的Offset不合法，太小或太大
	PULL_OFFSET_MOVED = 21,
	// Broker 查询消息未找到
	QUERY_NOT_FOUND = 22,
	// Broker 订阅关系解析失败
	SUBSCRIPTION_PARSE_FAILED = 23,
	// Broker 订阅关系不存在
	SUBSCRIPTION_NOT_EXIST = 24,
	// Broker 订阅关系不是最新的
	SUBSCRIPTION_NOT_LATEST = 25,
	// Broker 订阅组不存在
	SUBSCRIPTION_GROUP_NOT_EXIST = 26,
	// Producer 事务应该被提交
	TRANSACTION_SHOULD_COMMIT = 200,
	// Producer 事务应该被回滚
	TRANSACTION_SHOULD_ROLLBACK = 201,
	// Producer 事务状态未知
	TRANSACTION_STATE_UNKNOW = 202,
	// Producer ProducerGroup错误
	TRANSACTION_STATE_GROUP_WRONG = 203
};

enum ResponseCode {
	// 成功
	SUCCESS,
	// 发生了未捕获异常
	SYSTEM_ERROR,
	// 由于线程池拥堵，系统繁忙
	SYSTEM_BUSY,
	// 请求代码不支持
	REQUEST_CODE_NOT_SUPPORTED
};

#endif
