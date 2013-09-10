#ifndef DEFAULTMQPRODUCER_H
#define DEFAULTMQPRODUCER_H
#include "version.h"
#include "SendResult.h"
#include "common\message\message.h"

class DefaultMQProducerImpl;
class MessageQueue;
class MessageExt;
/**
 * ��Ϣ������
 * 
 */

class DefaultMQProducer {
private:
		/**
		 * һ�㷢��ͬ����Ϣ��Producer����Ϊͬһ��Group��Ӧ�ñ������ã�����֤����Ψһ
		 */
		std::string producerGroup;
		/**
		 * ֧���ڷ�����Ϣʱ�����Topic�����ڣ��Զ�����Topic������Ҫָ��Key
		 */
		std::string createTopicKey;
		/**
		 * ������Ϣ���Զ�����Topicʱ��Ĭ�϶�����
		 */
		int defaultTopicQueueNums;
		/**
		 * ������Ϣ��ʱ���������޸�
		 */
		int sendMsgTimeout;
		/**
		 * Message Body��С������ֵ����ѹ��
		 */
		int compressMsgBodyOverHowmuch;
		/**
		 * ��Ϣ�Ѿ��ɹ�д��Master������ˢ�̳�ʱ����ͬ����Slaveʧ�ܣ�����������һ��Broker���������޸�Ĭ��ֵ<br>
		 * ˳����Ϣ��Ч
		 */
		bool retryAnotherBrokerWhenNotStoreOK;
		/**
		 * �����Ϣ��С��Ĭ��512K
		 */
		int maxMessageSize;
		DefaultMQProducerImpl* defaultMQProducerImpl;
public:
		DefaultMQProducer();
		DefaultMQProducer(std::string& producerGroup);
		~DefaultMQProducer();
		void start();
		void shutdown();
		std::list<MessageQueue*>& fetchPublishMessageQueues(std::string& topic);
		SendResult_ptr send(Message_ptr msg);
		void sendOneway(Message_ptr msg);
		SendResult_ptr send(Message_ptr msg, MessageQueue* mq);
		void sendOneway(Message_ptr msg, MessageQueue* mq);
		void createTopic(std::string& key, std::string& newTopic, int queueNum);
		long earliestMsgStoreTime(MessageQueue mq);
		MessageExt viewMessage(std::string& msgId);
		//QueryResult queryMessage(std::string& topic, std::string& key, int maxNum, long begin, long end)
		std::string& getProducerGroup();
		void setProducerGroup(std::string& producerGroup);
		std::string& getCreateTopicKey();
		void setCreateTopicKey(std::string& createTopicKey);
		int getSendMsgTimeout();
		void setSendMsgTimeout(int sendMsgTimeout);
		int getCompressMsgBodyOverHowmuch();
		void setCompressMsgBodyOverHowmuch(int compressMsgBodyOverHowmuch);
		DefaultMQProducerImpl* getDefaultMQProducerImpl();
		bool isRetryAnotherBrokerWhenNotStoreOK();
		void setRetryAnotherBrokerWhenNotStoreOK(bool retryAnotherBrokerWhenNotStoreOK);
		int getMaxMessageSize();
		void setMaxMessageSize(int maxMessageSize);
		int getDefaultTopicQueueNums();
		void setDefaultTopicQueueNums(int defaultTopicQueueNums);
};

#endif
