#ifndef MQADMIN_H
#define MQADMIN_H

#include "common/message/MessageExt.h"
#include "common/message/MessageQueue.h"
/**
* MQ管理类接口
*/
class MQAdmin {
public:
	/**
	* 创建topic
	* 
	* @param key
	*            请向运维人员申请
	* @param newTopic
	*            要创建的新topic
	* @param queueNum
	*            新topic队列数
	* @param order
	*            是否是严格的顺序消息
	* @=0
	*/
	virtual void createTopic( std::string& key,  std::string& newTopic,  int queueNum)= 0;


	/**
	* 根据时间查询对应的offset，精确到毫秒<br>
	* P.S. 当前接口有较多IO开销，请勿频繁调用
	* 
	* @param mq
	*            队列
	* @param timestamp
	*            毫秒形式时间戳
	* @return 指定时间对应的offset
	* @=0
	*/
	virtual long searchOffset(MessageQueue mq,  long timestamp)=0;


	/**
	* 向服务器查询队列最大Offset PS: 最大Offset无对应消息，减1有消息
	* 
	* @param mq
	*            队列
	* @return 队列的最大Offset
	* @=0
	*/
	virtual long maxOffset( MessageQueue mq)=0;


	/**
	* 向服务器查询队列最小Offset PS: 最小Offset有对应消息
	* 
	* @param mq
	*            队列
	* @return 队列的最小Offset
	* @=0
	*/
	virtual long minOffset( MessageQueue mq)=0;


	/**
	* 向服务器查询队列保存的最早消息对应的存储时间
	* 
	* @param mq
	*            队列
	* @return 最早消息对应的存储时间，精确到毫秒
	* @=0
	*/
	virtual long earliestMsgStoreTime(MessageQueue mq)=0;


	/**
	* 根据消息ID，从服务器获取完整的消息
	* 
	* @param msgId
	* @return 完整消息
	* @throws InterruptedException
	* @throws MQBrokerException
	* @throws RemotingException
	* @=0
	*/
	//virtual MessageExt viewMessage(std::string& msgId)=0;


	/**
	* 根据消息Key查询消息
	* 
	* @param topic
	*            消息主题
	* @param key
	*            消息关键词
	* @param maxNum
	*            查询最大条数
	* @param begin
	*            起始时间戳
	* @param end
	*            结束时间戳
	* @return 查询结果
	* @=0
	* @throws InterruptedException
	*/
	//virtual QueryResult queryMessage( std::string& topic,  std::string& key,  int maxNum,  long begin, long end) =0;
};

#endif
