#ifndef MESSAGEQUEUELISTENER_H
#define MESSAGEQUEUELISTENER_H

/**
* ���б仯������
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