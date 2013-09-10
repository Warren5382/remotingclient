#ifndef MESSAGEQUEUELISTENER_H
#define MESSAGEQUEUELISTENER_H

/**
* 队列变化监听器
* 
* @author shijia.wxr<vintage.wang@gmail.com>
* @since 2013-7-24
*/
class MessageQueueListener {
public:
	void messageQueueChanged(std::string& topic, std::set<MessageQueue>* mqAll,
		std::set<MessageQueue> mqDivided);
};

#endif