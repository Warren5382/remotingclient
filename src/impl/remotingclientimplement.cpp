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
	//��������߳�
	boost::thread threadMonitor(Monitor,mReactor);
	threadMonitor.detach();
	std::cout << "create monitor" << std::endl;
}

void RemotingClientImplement::shutdown() {
	//�˴���Ҫ��취��ֹmonitor�߳�
}

Channel_ptr RemotingClientImplement::createChannel(std::string addr, int ID)
{
	Channel_var mychannel;
	bool IsUnderConnect = false;
	//������������
	try
	{
		mConnectMutex.lock();
		mychannel = mChannelTable->getChannel(addr);
		if(mychannel != NULL && mychannel->getState() == CHANNEL_CONNECTED) {
			return Channel_var::Duplicate(mychannel);
		}
		else if(mychannel != NULL && mychannel->getState() == CHANNEL_IDLE) {
			//��������
			IsUnderConnect = true;
		}
		else if(mychannel != NULL && mychannel->getState() == CHANNEL_ERROR) {
			//���ӳ���
			printf("create connect error\n");
			return Channel_var::Duplicate(mychannel);
		}
		if (!IsUnderConnect) {
			mychannel = new Channel();
			// ����socket���첽����
			mychannel->Start(addr);
			//��������ʱ������duplicate����varת��Ϊptr����
			mChannelTable->insertChannel(Channel_var::Duplicate(mychannel));
			//
			ClientEvent_var asyncHandle = new ClientEvent();
			asyncHandle->setAddr(addr);
			asyncHandle->setSocket(mychannel->getSocket());
			mReactor->RegisterLibevent(ClientEvent_var::Duplicate(asyncHandle));
			// connect����������Ҫʱ��
			std::cout << "try connect " << std::endl;
		}
		mConnectMutex.unlock();

		//�ȴ����ӳɹ��ź�
		std::cout << "waiting connect*********" << std::endl;
		mReactor->waitChannelNotify(addr);
		std::cout << "connected!" << std::endl;
		// connect����������, ���ߵȴ������ӳɹ�
		
		//û��copy����ʱ�����е�channel״̬�Ѿ����ġ�
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
	//�˴����ؼ�������Ϊ�������������������
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
	//ע�ᷴӦ���뷢���߳�ͨ�ŵ��첽��
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