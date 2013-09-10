#ifndef DEFAULTMQPRODUCER_H
#define DEFAULTMQPRODUCER_H
#include "version.h"
#include "SendResult.h"
#include "common\message\message.h"

class DefaultMQProducerImpl;
class MessageQueue;
class MessageExt;
/**
 * 消息生产者
 * 
 */

class DefaultMQProducer {
private:
		/**
		 * 一般发送同样消息的Producer，归为同一个Group，应用必须设置，并保证命名唯一
		 */
		std::string producerGroup;
		/**
		 * 支持在发送消息时，如果Topic不存在，自动创建Topic，但是要指定Key
		 */
		std::string createTopicKey;
		/**
		 * 发送消息，自动创建Topic时，默认队列数
		 */
		int defaultTopicQueueNums;
		/**
		 * 发送消息超时，不建议修改
		 */
		int sendMsgTimeout;
		/**
		 * Message Body大小超过阀值，则压缩
		 */
		int compressMsgBodyOverHowmuch;
		/**
		 * 消息已经成功写入Master，但是刷盘超时或者同步到Slave失败，则尝试重试另一个Broker，不建议修改默认值<br>
		 * 顺序消息无效
		 */
		bool retryAnotherBrokerWhenNotStoreOK;
		/**
		 * 最大消息大小，默认512K
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
