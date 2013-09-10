#include "remotingclientimplement.h"
#include "tools/clientEvent.h"
#include "socket/channel.h"
#include <iostream>

RemotingClientImplement::RemotingClientImplement()
{
	mChannelTable = new ChannelTable();
	mReactor = new Reactor(mChannelTable);
	std::cout << "construct a remotingclientimplement" << std::endl;
}

RemotingClientImplement::~RemotingClientImplement()
{
	ChannelTable::Release(mChannelTable);
	if(mReactor) {
		delete mReactor;
	}
	std::cout << "deconstruct a remotingclientimplement" << std::endl;
}

void RemotingClientImplement::start() {
	//创建监控线程
	boost::thread threadMonitor(Monitor,mReactor);
	threadMonitor.detach();
	std::cout << "create monitor" << std::endl;
}

void RemotingClientImplement::shutdown() {
	//此处需要想办法终止monitor线程
}

Channel_ptr RemotingClientImplement::createChannel(std::string addr, int ID)
{
	Channel_var mychannel;
	bool IsUnderConnect = false;
	//加锁创建连接
	try
	{
		mConnectMutex.lock();
		mychannel = mChannelTable->getChannel(addr);
		if(mychannel != NULL && mychannel->getState() == CHANNEL_CONNECTED) {
			return Channel_var::Duplicate(mychannel);
		}
		else if(mychannel != NULL && mychannel->getState() == CHANNEL_IDLE) {
			//正在连接
			IsUnderConnect = true;
		}
		else if(mychannel != NULL && mychannel->getState() == CHANNEL_ERROR) {
			//连接出错
			printf("create connect error\n");
			return Channel_var::Duplicate(mychannel);
		}
		if (!IsUnderConnect) {
			mychannel = new Channel();
			// 创建socket并异步连接
			mychannel->Start(addr);
			//容器插入时，调用duplicate，将var转化为ptr插入
			mChannelTable->insertChannel(Channel_var::Duplicate(mychannel));
			//
			ClientEvent_var asyncHandle = new ClientEvent();
			asyncHandle->setAddr(addr);
			asyncHandle->setSocket(mychannel->getSocket());
			mReactor->RegisterLibevent(ClientEvent_var::Duplicate(asyncHandle));
			// connect建立连接需要时间
			std::cout << "try connect " << std::endl;
		}
		mConnectMutex.unlock();

		//等待连接成功信号
		std::cout << "waiting connect*********" << std::endl;
		mReactor->waitChannelNotify(addr);
		std::cout << "connected!" << std::endl;
		// connect立即连接上, 或者等待到连接成功
		
		//没有copy，此时容器中的channel状态已经更改。
		return Channel_var::Duplicate(mychannel);
	}
	catch (myException& e) {
		mConnectMutex.unlock();
		mychannel->setState(CHANNEL_ERROR);
		std::cerr << *boost::get_error_info<boost::errinfo_api_function>(e);
		std::cerr << *boost::get_error_info<boost::errinfo_at_line>(e);
		return NULL;
	}
}

Channel_ptr RemotingClientImplement::getAndCreateChannel(std::string addr, int ID)
{
	//此处不必加锁，因为后续创建处还会加锁的
	Channel_var mychannel;
	mychannel = mChannelTable->getChannel(addr);
	if(mychannel != NULL) {
		return Channel_var::Duplicate(mychannel);
	}
	else {
		mychannel = createChannel(addr, ID);
		return Channel_var::Duplicate(mychannel);
	}
}

void RemotingClientImplement::sendData(Channel_ptr mychannel, RemotingCommand_ptr request)
{
	
	ByteBuffer_var toSendData = request->encode();
	mychannel->SendData(toSendData->Inout(), toSendData->Size());
}

RemotingCommand_ptr RemotingClientImplement::invokeSync(std::string addr, RemotingCommand_ptr request, int timeout)
{
	//注册反应器与发送线程通信的异步器
	mReactor->RegisterWorkerNotify(request->getOpaque());
	Channel_var mychannel = getAndCreateChannel(addr, request->getOpaque());
	sendData(Channel_var::Duplicate(mychannel), RemotingCommand_var::Duplicate(request));
	mReactor->waitWorkerNotify(request->getOpaque());
	RemotingCommand_var response = mReactor->getResponse();
	return RemotingCommand_var::Duplicate(response);
}

void RemotingClientImplement::registerProcessor()
{

}

std::list<std::string>& RemotingClientImplement::getNameServerAddressList()
{
	std::list<std::string> templist;
	return templist;
}

void RemotingClientImplement::updateNameServerAddressList( std::list<std::string>& addrs )
{

}