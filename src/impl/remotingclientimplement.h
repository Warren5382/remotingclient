#ifndef REMOTINGCLIENTIMPLEMENT_H
#define REMOTINGCLIENTIMPLEMENT_H

#include "version.h"
#include "RemotingClient.h"
#include "Socket/Channel.h"
#include "protocol/RemotingCommand.h"
#include "reactor/Reactor.h"


class RemotingClientImplement : public RemotingClient{
	public:
		RemotingClientImplement();
		~RemotingClientImplement();
		void start();
		void shutdown();
		RemotingCommand_ptr invokeSync(std::string addr, RemotingCommand_ptr request, int timeout);
		void updateNameServerAddressList(std::list<std::string>& addrs);
		std::list<std::string>& getNameServerAddressList();
		void registerProcessor();
	private:
		// 异步反应器
		Reactor* mReactor;
		// 
		ChannelTable_var mChannelTable;
		/*
		 * 互斥锁,用于创建连接
		 */
		boost::mutex mConnectMutex;
		/*
		 * 互斥锁，用于更改channle状态
		 */
		boost::mutex mChangeStateMutex;
		/*
		 *获取套接字
		 */
		Channel_ptr getAndCreateChannel(std::string addr, int ID);
		/*
		 *如果table中没有该服务器地址的连接，则创建连接
		 */
		Channel_ptr createChannel(std::string addr, int ID);
		/*
		 *发送数据
		 */
		void sendData(Channel_ptr, RemotingCommand_ptr request);
};


#endif
