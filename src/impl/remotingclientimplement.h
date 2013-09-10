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
		// �첽��Ӧ��
		Reactor* mReactor;
		// 
		ChannelTable_var mChannelTable;
		/*
		 * ������,���ڴ�������
		 */
		boost::mutex mConnectMutex;
		/*
		 * �����������ڸ���channle״̬
		 */
		boost::mutex mChangeStateMutex;
		/*
		 *��ȡ�׽���
		 */
		Channel_ptr getAndCreateChannel(std::string addr, int ID);
		/*
		 *���table��û�и÷�������ַ�����ӣ��򴴽�����
		 */
		Channel_ptr createChannel(std::string addr, int ID);
		/*
		 *��������
		 */
		void sendData(Channel_ptr, RemotingCommand_ptr request);
};


#endif
