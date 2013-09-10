#ifndef TOPICPUBLISHINFO_H
#define TOPICPUBLISHINFO_H
#include "version.h"
#include "common/message/MessageQueue.h"

class TopicPublishInfo {
	private:
		bool orderTopic;
		list<MessageQueue*> messageQueueList;
		LWPR::AtomicInteger sendWhichQueue;
	public:
		TopicPublishInfo() : orderTopic(false), sendWhichQueue(0) {
		};
		~TopicPublishInfo() {
		};
		bool isOrderTopic() {
			return orderTopic;
		};
		bool ok() {
			return !this->messageQueueList.empty();
		};
		void setOrderTopic(bool orderTopic) {
			this->orderTopic = orderTopic;
		};
		list<MessageQueue*>& getMessageQueueList() {
			return this->messageQueueList;
		};
		void setMessageQueueList() {
		};
		/**
		 * 如果lastBrokerName不为null，则寻找其他不同的messagequeue
		 */
		MessageQueue* selectOneMessageQueue(std::string& lastBrokerName) {
			int size = this->messageQueueList.size();
			if(lastBrokerName.empty()) {
				int index = this->sendWhichQueue.GetAndIncrement();
				int pos = index%size;
				list<MessageQueue*>::iterator it = this->messageQueueList.begin();
				for(int i = 0; i < pos; i++)
					it++;
				return (*it);
			}
			else {
				int index = this->sendWhichQueue.GetAndIncrement();
				for (int i = 0; i < size; i++) {
					int pos = (index++)%size;
					list<MessageQueue*>::iterator it = this->messageQueueList.begin();
					for(int i = 0; i < pos; i++)
						it++;
					MessageQueue* mq = (*it);
					if (mq->getBrokerName() != lastBrokerName) {
						return mq;
					}
				}
				return NULL;
			}
		};
};

#endif
		


