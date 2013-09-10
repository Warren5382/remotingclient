#include <iostream>
#include "version.h"
#include "mqclientapiimpl.h"
#include "protocol\heartbeatdata.h"
/*
void sendMessage(void* args) {
	RemotingClient* myclient = (RemotingClientImplement*)args;
	SendMessageRequestHeader* commandheader = new SendMessageRequestHeader();
	commandheader->setProducerGroup("abc");
	commandheader->setTopic("test");
	commandheader->setDefaultTopic("TBW102");
	commandheader->setDefaultTopicQueueNums(4);
	commandheader->setQueueId(3);
	commandheader->setSysFlag(0);
	commandheader->setBornTimestamp(1374827230157);
	commandheader->setFlag(0);
	commandheader->setProperties("DELAY\11\2TAGS\1TAG1 TAG2\2WAIT\1true\2KEYS\1100200300\2");

	RemotingCommand_var testcommand = RemotingCommand::createRequestCommand(10, commandheader);
	testcommand->setBody("good boy haha");
	RemotingCommand_var response = myclient->invokeSync("127.0.0.1", testcommand, 3);
	std::cout << "body" << response->getBody() << std::endl;
};

void receMessage(void* args) {
	RemotingClient* myclient = (RemotingClientImplement*)args;
	PullMessageRequestHeader* commandheader = new PullMessageRequestHeader();

	commandheader->setConsumerGroup("abc");
	commandheader->setTopic("test");
	commandheader->setQueueId(3);
	commandheader->setSysFlag(0);
	commandheader->setQueueOffset(15);

	RemotingCommand_var testcommand = RemotingCommand::createRequestCommand(11, commandheader);
	//testcommand->setBody("good boy haha");
	RemotingCommand_var response = myclient->invokeSync("127.0.0.1", testcommand, 3);
	std::cout << "body" << response->getBody() << std::endl;
};
*/
int main(void) {
	MQClientAPIImpl client;
	client.start();
/*
	SendMessageRequestHeader* commandheader = new SendMessageRequestHeader();
	commandheader->setProducerGroup("CLIENT_INNER_PRODUCER");
	commandheader->setTopic("test");
	commandheader->setDefaultTopic("TBW102");
	commandheader->setDefaultTopicQueueNums(4);
	commandheader->setQueueId(3);
	commandheader->setSysFlag(0);
	commandheader->setBornTimestamp(1374827230157);
	commandheader->setFlag(0);
	commandheader->setProperties("DELAY\11\2TAGS\1TAG1 TAG2\2WAIT\1true\2KEYS\1100200300\2");
	client.sendMessage("127.0.0.1:10911", "name", commandheader, 3);
*/	

	HeartbeatData* heartbeatData = new HeartbeatData();
	client.sendHeartbeat("127.0.0.1:10911", heartbeatData, 3);

	Sleep(3000);
	client.sendHeartbeat("127.0.0.1:10911", heartbeatData, 3);

/*
	PullMessageRequestHeader* commandheader2 = new PullMessageRequestHeader();
	commandheader2->setConsumerGroup("example_pull_consumer_group");
	commandheader2->setTopic("test");
	commandheader2->setQueueId(3);
	commandheader2->setSysFlag(0);
	commandheader2->setQueueOffset(15);
	client.pullMessage("127.0.0.1:10911", commandheader2, 3);
*/
	

	int a;
	std::cin >> a ;
	client.shutdown();
	return 0;
}
