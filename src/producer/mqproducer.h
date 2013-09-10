#ifndef MQPRODUCER_H
#define MQPRODUCER_H
#include "MQAdmin.h"
#include "common/message/MessageQueue.h"
#include "SendResult.h"
/**
* ��Ϣ������
* 
*/
class MQProducer : public MQAdmin {
public:
	/**
	* ��������
	* 
	* @throws MQClientException
	*/
	virtual void start()=0;


	/**
	* �رշ���һ���رգ��˶��󽫲�����
	*/
	virtual void shutdown()=0;


	/**
	* ����topic��ȡ��Ӧ��MessageQueue�������˳����Ϣ������˳����Ϣ���÷���
	* 
	* @param topic
	*            ��ϢTopic
	* @return ���ض��м���
	* @throws MQClientException
	*/
	virtual std::list<MessageQueue*>& fetchPublishMessageQueues(std::string& topic)=0;


	/**
	* ������Ϣ��ͬ������
	* 
	* @param msg
	*            ��Ϣ
	* @return ���ͽ��
	* @throws InterruptedException
	* @throws MQBrokerException
	* @throws RemotingException
	* @throws MQClientException
	*/
	virtual SendResult send(Message msg)=0;

	/**
	* ������Ϣ��Oneway��ʽ����������Ӧ���޷���֤��Ϣ�Ƿ�ɹ����������
	* 
	* @param msg
	*            ��Ϣ
	* @throws MQClientException
	* @throws RemotingException
	* @throws InterruptedException
	*/
	virtual void sendOneway(Message msg)=0;


	/**
	* ��ָ�����з�����Ϣ��ͬ������
	* 
	* @param msg
	*            ��Ϣ
	* @param mq
	*            ����
	* @return ���ͽ��
	* @throws InterruptedException
	* @throws MQBrokerException
	* @throws RemotingException
	* @throws MQClientException
	*/
	virtual SendResult send(Message msg, MessageQueue* mq)=0;

	/**
	* ��ָ�����з�����Ϣ��Oneway��ʽ����������Ӧ���޷���֤��Ϣ�Ƿ�ɹ����������
	* 
	* @param msg
	*            ��Ϣ
	* @param mq
	*            ����
	* @throws MQClientException
	* @throws RemotingException
	* @throws InterruptedException
	*/
	virtual void sendOneway(Message msg, MessageQueue mq)=0;
};

#endif