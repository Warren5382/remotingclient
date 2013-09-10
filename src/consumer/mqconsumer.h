#ifndef MQCONSUMER_H
#define MQCONSUMER_H
#include "../MQAdmin.h"

class MQConsumer : public MQAdmin {
public:
	/**
	* Consumer消费失败的消息可以选择重新发回到服务器端，并延时消费<br>
	* 会首先尝试将消息发回到消息之前存储的主机，此时只传送消息Offset，消息体不传送，不会占用网络带宽<br>
	* 如果发送失败，会自动重试发往其他主机，此时消息体也会传送<br>
	* 重传回去的消息只会被当前Consumer Group消费。
	* 
	* @param msg
	* @param delayLevel
	* @throws InterruptedException
	* @throws MQBrokerException
	* @throws RemotingException
	* @throws MQClientException
	*/
	virtual void sendMessageBack(MessageExt msg, int delayLevel)=0;


	/**
	* 根据topic获取对应的MessageQueue，是可被订阅的队列<br>
	* P.S 从Consumer Cache中拿数据，可以频繁调用。Cache中数据大约30秒更新一次
	* 
	* @param topic
	*            消息Topic
	* @return 返回队列集合
	* @throws MQClientException
	*/
	virtual std::set<MessageQueue*>& fetchSubscribeMessageQueues(std::string& topic)=0;
};
#endif