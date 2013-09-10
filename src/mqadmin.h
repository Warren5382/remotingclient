#ifndef MQADMIN_H
#define MQADMIN_H

#include "common/message/MessageExt.h"
#include "common/message/MessageQueue.h"
/**
* MQ������ӿ�
*/
class MQAdmin {
public:
	/**
	* ����topic
	* 
	* @param key
	*            ������ά��Ա����
	* @param newTopic
	*            Ҫ��������topic
	* @param queueNum
	*            ��topic������
	* @param order
	*            �Ƿ����ϸ��˳����Ϣ
	* @=0
	*/
	virtual void createTopic( std::string& key,  std::string& newTopic,  int queueNum)= 0;


	/**
	* ����ʱ���ѯ��Ӧ��offset����ȷ������<br>
	* P.S. ��ǰ�ӿ��н϶�IO����������Ƶ������
	* 
	* @param mq
	*            ����
	* @param timestamp
	*            ������ʽʱ���
	* @return ָ��ʱ���Ӧ��offset
	* @=0
	*/
	virtual long searchOffset(MessageQueue mq,  long timestamp)=0;


	/**
	* ���������ѯ�������Offset PS: ���Offset�޶�Ӧ��Ϣ����1����Ϣ
	* 
	* @param mq
	*            ����
	* @return ���е����Offset
	* @=0
	*/
	virtual long maxOffset( MessageQueue mq)=0;


	/**
	* ���������ѯ������СOffset PS: ��СOffset�ж�Ӧ��Ϣ
	* 
	* @param mq
	*            ����
	* @return ���е���СOffset
	* @=0
	*/
	virtual long minOffset( MessageQueue mq)=0;


	/**
	* ���������ѯ���б����������Ϣ��Ӧ�Ĵ洢ʱ��
	* 
	* @param mq
	*            ����
	* @return ������Ϣ��Ӧ�Ĵ洢ʱ�䣬��ȷ������
	* @=0
	*/
	virtual long earliestMsgStoreTime(MessageQueue mq)=0;


	/**
	* ������ϢID���ӷ�������ȡ��������Ϣ
	* 
	* @param msgId
	* @return ������Ϣ
	* @throws InterruptedException
	* @throws MQBrokerException
	* @throws RemotingException
	* @=0
	*/
	//virtual MessageExt viewMessage(std::string& msgId)=0;


	/**
	* ������ϢKey��ѯ��Ϣ
	* 
	* @param topic
	*            ��Ϣ����
	* @param key
	*            ��Ϣ�ؼ���
	* @param maxNum
	*            ��ѯ�������
	* @param begin
	*            ��ʼʱ���
	* @param end
	*            ����ʱ���
	* @return ��ѯ���
	* @=0
	* @throws InterruptedException
	*/
	//virtual QueryResult queryMessage( std::string& topic,  std::string& key,  int maxNum,  long begin, long end) =0;
};

#endif
