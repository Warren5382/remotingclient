#ifndef MQCLIENTAPIIMPL_H
#define MQCLIENTAPIIMPL_H
#include "version.h"
#include "commandheader\commandcustomheader.h"
#include "consumer\PullResult.h"
#include "protocol\remotingcommand.h"

class RemotingClient;
class TopicRouteData;
class HeartbeatData;
class RemotingClient;

class MQClientAPIImpl {
public:
	MQClientAPIImpl();
	~MQClientAPIImpl();
	RemotingClient* getRemotingClient();
	std::string fetchNameServerAddr();
	void updateNameServerAddressList(std::string& addrs);
	TopicRouteData* getTopicRouteInfoFromNameServer(std::string& topic);
	void start();
	void shutdown();
	void createSubscriptionGroup(std::string addr, int timeout);
	void createTopic(std::string addr, std::string defaultTopic, int timeout);
	void sendMessage(std::string addr, std::string brokerName, SendMessageRequestHeader_ptr requestHeader, int timeout);
	PullResult_ptr pullMessage(std::string addr, PullMessageRequestHeader_ptr requestHeader, int timeout);
	void sendHeartbeat(std::string addr, HeartbeatData* heartbeatData, int timeout);
private:
	RemotingClient* remotingClient;
	std::string nameSrvAddr;
	PullResult_ptr processPullResponse(RemotingCommand_ptr response);
};
#endif