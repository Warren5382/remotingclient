#include <iostream>
#include <fstream>
#include "producer\defaultmqproducer.h"
#include "protocol\remotingcommand.h"
#include "common\message\message.h"
#include "producer\SendResult.h"
#include "consumer\defaultmqpullconsumer.h"
#include "consumer\PullResult.h"

int main(void) {
	int a;
	std::string test = "unique_name";
	std::string topic = "TopicTestpikong";
	
	/*
	DefaultMQProducer* producer = new DefaultMQProducer(test);
	producer->start();

	Message_var msg = new Message(topic, "TagA", "KEY1", "hello rocketmq");
	SendResult_var ret = producer->send(Message_var::Duplicate(msg));
	producer->shutdown();
*/


	DefaultMQPullConsumer* consumer = new DefaultMQPullConsumer(test);
	consumer->start();

	std::set<MessageQueue*> mqs;
	consumer->fetchSubscribeMessageQueues(topic, mqs);
	std::set<MessageQueue*>::iterator it = mqs.begin();
	std::string subexe = "";
	printf("******************begin consumer\n");
	for(; it != mqs.end(); it++) {
		MessageQueue* mq = *it;
		PullResult_var ret = consumer->pullBlockIfNotFound(mq, subexe, 0, 32);
		delete mq;
	}
	mqs.clear();
	consumer->shutdown();
	
	
	scanf("%d", &a);
	delete consumer;
	//delete producer;
	return 0;
}