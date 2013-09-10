#include <fstream>
#include "mqclientapiimpl.h"
#include "remotingclient.h"
#include "protocol\remotingcommand.h"
#include "impl\remotingclientimplement.h"
#include "protocol\topicroutedata.h"
#include "protocol\heartbeatdata.h"
#include "protocol\mqprotos.h"
#include "commandheader\commandcustomheader.h"

MQClientAPIImpl::MQClientAPIImpl() : nameSrvAddr("127.0.0.1:9876")
{
	remotingClient = new RemotingClientImplement();
}

MQClientAPIImpl::~MQClientAPIImpl()
{
	delete remotingClient;
}

RemotingClient* MQClientAPIImpl::getRemotingClient()
{
	return remotingClient;
}

std::string MQClientAPIImpl::fetchNameServerAddr()
{
	return nameSrvAddr;
}

void MQClientAPIImpl::updateNameServerAddressList( std::string& addrs )
{

}

void MQClientAPIImpl::start()
{
	remotingClient->start();
}

void MQClientAPIImpl::shutdown()
{
	remotingClient->shutdown();
}

void MQClientAPIImpl::createSubscriptionGroup( std::string addr, int timeout )
{

}

void MQClientAPIImpl::createTopic( std::string addr, std::string defaultTopic, int timeout )
{

}

void MQClientAPIImpl::sendMessage( std::string addr, std::string brokerName, SendMessageRequestHeader_ptr requestHeader, int timeout )
{
	RemotingCommand_var request = RemotingCommand::createRequestCommand(MQRequestCode::SEND_MESSAGE, requestHeader);
	RemotingCommand_var response = remotingClient->invokeSync(addr, RemotingCommand_var::Duplicate(request), timeout);
}

PullResult_ptr MQClientAPIImpl::pullMessage( std::string addr, PullMessageRequestHeader_ptr requestHeader, int timeout)
{
	RemotingCommand_var request = RemotingCommand::createRequestCommand(MQRequestCode::PULL_MESSAGE, requestHeader);
	RemotingCommand_var response = remotingClient->invokeSync(addr, RemotingCommand_var::Duplicate(request), timeout);
	return processPullResponse(RemotingCommand_var::Duplicate(response));
}

void MQClientAPIImpl::sendHeartbeat( std::string addr, HeartbeatData* heartbeatData, int timeout )
{
	RemotingCommand_var request = RemotingCommand::createRequestCommand(HEART_BEAT,NULL);
	/*
	std::ifstream in("D:/CplusplusPro/RemotingClient/cpp/heartbeatdata.txt");
	std::istreambuf_iterator<char> beg(in), end;
	std::string headerData(beg, end);
	in.close();
	request->setBody(headerData);*/
	
	std::string data = heartbeatData->Serialize();
	Byte_var bodydata = new Byte(data.length());
	bodydata->setBuffer(data.c_str(), data.length());
	request->setBody(Byte_var::Duplicate(bodydata));
	RemotingCommand_var response = remotingClient->invokeSync(addr, RemotingCommand_var::Duplicate(request), timeout);
}

TopicRouteData* MQClientAPIImpl::getTopicRouteInfoFromNameServer( std::string& topic )
{
	GetRouteInfoRequestHeader_var requestHeader = new GetRouteInfoRequestHeader();
	requestHeader->setTopic(topic);
	RemotingCommand_var request = RemotingCommand::createRequestCommand(GET_ROUTEINTO_BY_TOPIC, GetRouteInfoRequestHeader_var::Duplicate(requestHeader));
	RemotingCommand_var response = remotingClient->invokeSync(fetchNameServerAddr(), RemotingCommand_var::Duplicate(request), 3);
	if(response->getCode() != 0) {
		return NULL;
	}
	TopicRouteData* topicRouteData = new TopicRouteData();
	/*
	std::ifstream in("D:/CplusplusPro/RemotingClient/cpp/topicroutedata.txt");
	std::istreambuf_iterator<char> beg(in), end;
	std::string topicData(beg, end);
	in.close();
	*/
	Byte_var bodydata = response->getBody();
	std::string data(bodydata->getBuffer());
	//将brokerid变为string类型
	data.insert(32, "*");
	data.insert(34, "*");
	data[32] = 34;
	data[34] = 34;
	topicRouteData->DeSerialize(data.c_str());
	return topicRouteData;
}

PullResult_ptr MQClientAPIImpl::processPullResponse( RemotingCommand_ptr response )
{
	PullStatus pullStatus = NO_NEW_MSG;
	switch(response->getCode()) {
		case ResponseCode::SUCCESS:
			pullStatus = PullStatus::FOUND;
			break;
		case MQResponseCode::PULL_NOT_FOUND:
			pullStatus = PullStatus::NO_NEW_MSG;
			break;
		case MQResponseCode::PULL_RETRY_IMMEDIATELY:
			pullStatus = PullStatus::NO_MATCHED_MSG;
			break;
		case MQResponseCode::PULL_OFFSET_MOVED:
			pullStatus = PullStatus::OFFSET_ILLEGAL;
			break;

		default:
			return NULL;
	}

	
	PullMessageResponseHeader_var responseHeader = new PullMessageResponseHeader();
	response->decodeCommandCustomHeader(PullMessageResponseHeader_var::Duplicate(responseHeader));
	printf("\n**********conume an message***********\n");
	printf("%ld, %ld, %d\n", responseHeader->getMinOffset(), responseHeader->getMaxOffset(), response->getBody()->size());
	PullResult_var ret = new PullResult(pullStatus, responseHeader->getNextBeginOffset(), 
		responseHeader->getMinOffset(), responseHeader->getMaxOffset(),
		responseHeader->getSuggestWhichBrokerId(), response->getBody());
	return PullResult_var::Duplicate(ret);

}