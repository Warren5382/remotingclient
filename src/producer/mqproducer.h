#ifndef MQPRODUCER_H
#define MQPRODUCER_H
#include "MQAdmin.h"
#include "common/message/MessageQueue.h"
#include "SendResult.h"
/**
* 消息生产者
* 
*/
class MQProducer : public MQAdmin {
public:
	/**
	* 启动服务
	* 
	* @throws MQClientException
	*/
	virtual void start()=0;


	/**
	* 关闭服务，一旦关闭，此对象将不可用
	*/
	virtual void shutdown()=0;


	/**
	* 根据topic获取对应的MessageQueue，如果是顺序消息，则按照顺序消息配置返回
	* 
	* @param topic
	*            消息Topic
	* @return 返回队列集合
	* @throws MQClientException
	*/
	virtual std::list<MessageQueue*>& fetchPublishMessageQueues(std::string& topic)=0;


	/**
	* 发送消息，同步调用
	* 
	* @param msg
	*            消息
	* @return 发送结果
	* @throws InterruptedException
	* @throws MQBrokerException
	* @throws RemotingException
	* @throws MQClientException
	*/
	virtual SendResult send(Message msg)=0;

	/**
	* 发送消息，Oneway形式，服务器不应答，无法保证消息是否成功到达服务器
	* 
	* @param msg
	*            消息
	* @throws MQClientException
	* @throws RemotingException
	* @throws InterruptedException
	*/
	virtual void sendOneway(Message msg)=0;


	/**
	* 向指定队列发送消息，同步调用
	* 
	* @param msg
	*            消息
	* @param mq
	*            队列
	* @return 发送结果
	* @throws InterruptedException
	* @throws MQBrokerException
	* @throws RemotingException
	* @throws MQClientException
	*/
	virtual SendResult send(Message msg, MessageQueue* mq)=0;

	/**
	* 向指定队列发送消息，Oneway形式，服务器不应答，无法保证消息是否成功到达服务器
	* 
	* @param msg
	*            消息
	* @param mq
	*            队列
	* @throws MQClientException
	* @throws RemotingException
	* @throws InterruptedException
	*/
	virtual void sendOneway(Message msg, MessageQueue mq)=0;
};

#endif