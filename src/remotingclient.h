#ifndef REMOTINGCLIENT_H
#define REMOTINGCLIENT_H

#include "version.h"
#include "protocol/RemotingCommand.h"
class RemotingClient{
	public:
		virtual void start()=0;
		virtual void updateNameServerAddressList(std::list<std::string>& addrs)=0;
		virtual std::list<std::string>& getNameServerAddressList()=0;
		virtual RemotingCommand_ptr invokeSync(std::string addr, RemotingCommand_ptr request, int timeout)=0;
		virtual void registerProcessor()=0;
		virtual void shutdown()=0;
};
#endif
