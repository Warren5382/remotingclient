#ifndef MQCONSUMER_H
#define MQCONSUMER_H
#include "../MQAdmin.h"

class MQConsumer : public MQAdmin {
public:
	/**
	* Consumer����ʧ�ܵ���Ϣ����ѡ�����·��ص��������ˣ�����ʱ����<br>
	* �����ȳ��Խ���Ϣ���ص���Ϣ֮ǰ�洢����������ʱֻ������ϢOffset����Ϣ�岻���ͣ�����ռ���������<br>
	* �������ʧ�ܣ����Զ����Է���������������ʱ��Ϣ��Ҳ�ᴫ��<br>
	* �ش���ȥ����Ϣֻ�ᱻ��ǰConsumer Group���ѡ�
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
	* ����topic��ȡ��Ӧ��MessageQueue���ǿɱ����ĵĶ���<br>
	* P.S ��Consumer Cache�������ݣ�����Ƶ�����á�Cache�����ݴ�Լ30�����һ��
	* 
	* @param topic
	*            ��ϢTopic
	* @return ���ض��м���
	* @throws MQClientException
	*/
	virtual std::set<MessageQueue*>& fetchSubscribeMessageQueues(std::string& topic)=0;
};
#endif