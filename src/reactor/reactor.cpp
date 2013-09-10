#include "reactor/reactor.h"
#include "bytebuffer/bytebuffer.h"
#include "protocol/remotingcommand.h"
#include <fstream>

Reactor::Reactor(ChannelTable_ptr ctable) {
	mChannelTable = ctable;
	scanUnusedChannelFreq = 0;
}

Reactor::~Reactor() {
	ChannelTable::Release(mChannelTable);
	{
		while(!this->LibeventContainer.empty()) {
			ClientEvent_ptr clientevent = this->LibeventContainer.front();
			ClientEvent::Release(clientevent);
			this->LibeventContainer.pop();
		}
	}
	{
		boost::unordered_map<std::string, CountDown_ptr>::iterator it = this->ChannelNotifier.begin();
		for(; it != this->ChannelNotifier.end(); it++) {
			CountDown_ptr countdown = it->second;
			CountDown::Release(countdown);
		}
		this->ChannelNotifier.clear();
	}
	{
		boost::unordered_map<int, CountDown_ptr>::iterator it = this->WorkerNotifier.begin();
		for(; it != this->WorkerNotifier.end(); it++) {
			CountDown_ptr countdown = it->second;
			CountDown::Release(countdown);
		}
		this->WorkerNotifier.clear();
	}
	{
		this->SocktoChannel.clear();
	}
	RemotingCommand::Release(mResponse);
}

void Reactor::start() {
	struct timeval tv = {1,0};
	//int evthread_use_windows_threads(void);//win上设置,多线程安全
	this->mBase = event_base_new();

	struct event* timer = event_new(this->mBase, -1, EV_TIMEOUT|EV_PERSIST, processTimeEclapsed, this);
	event_add(timer, &tv);

	event_base_dispatch(this->mBase);
	event_base_free(this->mBase);
}

void Reactor::RegisterWorkerNotify(int workID) {
	CountDown_var clientHandle = new CountDown();
	// 在容器中插入
	this->WorkerNotifier[workID] = CountDown_var::Duplicate(clientHandle);
}

void Reactor::RegisterLibevent(ClientEvent_ptr clientHandle) {
	try {
		this->LibeventContainer.push(clientHandle);
		CountDown_var countdown = new CountDown();
		this->ChannelNotifier[clientHandle->getAddr()] = CountDown_var::Duplicate(countdown);
		this->SocktoChannel[clientHandle->getSocket()] = clientHandle->getAddr();
	}
	catch (myException e) {
	}
}

void Reactor::notifyChannel(evutil_socket_t sock) {
	std::string addr = findaddrbysock(sock);
	boost::unordered_map<std::string,CountDown_ptr>::iterator it;
	it = this->ChannelNotifier.find(addr);
	if(it == this->ChannelNotifier.end()) {
		printf("cannnot find the channel's notifer");
		return (void)0;
	}
	it->second->Set();
}

void Reactor::notifyWorker(int id) {
	boost::unordered_map<int, CountDown_ptr>::iterator it;
	it = this->WorkerNotifier.find(id);
	if (it == this->WorkerNotifier.end()) {
		printf("cannot find the id's notifer id = %d\n", id);
		return (void)0;
	}
	else it->second->Set();
}

void Reactor::waitChannelNotify(std::string addr) {
	boost::unordered_map<std::string, CountDown_ptr>::iterator it;
	it = this->ChannelNotifier.find(addr);
	if (it == this->ChannelNotifier.end()) {
		printf("cannnot find the channel's notifer");
		return (void)0;
	}
	it->second->Wait();
	it->second->Reset();
}

bool Reactor::waitWorkerNotify( int ID)
{
	bool flag =false;
	boost::unordered_map<int, CountDown_ptr>::iterator it;
	it = this->WorkerNotifier.find(ID);
	if (it == this->WorkerNotifier.end()) {
		printf("cannnot find the worker's notifer");
		return flag;
	}
	flag = it->second->Wait(10);
	it->second->Reset();
	return flag;
}

void Reactor::scanLibeventContainer() {
	ClientEvent_var clientHandle;
	while(!LibeventContainer.empty()) {
		clientHandle = ClientEvent_var::Duplicate(LibeventContainer.front());
		//这里担心会内存泄露
		LibeventContainer.pop();
		struct event* event_write = event_new(mBase, clientHandle->getSocket(), EV_TIMEOUT|EV_WRITE, enableWrite, this);
		ClientEvent::Release(clientHandle);
		if(event_write == NULL) {
			//throw myException() << boost::errinfo_api_function("event_new error in ClientEvent::createReadandWrite") << boost::errinfo_at_line(20);
			std::cout << "createwrite error" << std::endl;
			return (void)0;
		}
		addWritable(event_write);
	}
}

void Reactor::addWritable(struct event* ec) {
	struct timeval tv = {3,0};
	int ret = event_add(ec, &tv);
	if (ret < 0) {
		printf("add write error\n");
	}
}

struct event_base* Reactor::getBaseEvent()
{
	return this->mBase;
}

void Reactor::addReadable(evutil_socket_t socket)
{
	struct event* event_read = event_new(mBase, socket, EV_READ|EV_PERSIST, enableRead, this);
	if(event_read == NULL) {
		//throw myException() << boost::errinfo_api_function("event_new error in ClientEvent::createReadandWrite") << boost::errinfo_at_line(20);
		std::cout << "create read error" << std::endl;
	}
	int ret = event_add(event_read, 0);
	if (ret < 0) {
		printf("add write error\n");
	}
}

void Reactor::setResponse( RemotingCommand_ptr res )
{
	this->mResponse = res;
}

RemotingCommand_ptr Reactor::getResponse() {
	return RemotingCommand_var::Duplicate(this->mResponse);
}

std::string Reactor::findaddrbysock(evutil_socket_t sock)
{
	boost::unordered_map<evutil_socket_t,std::string>::iterator itone;
	itone = this->SocktoChannel.find(sock);
	if (itone == this->SocktoChannel.end()) {
		printf("cannot find the sock's channel");
		return NULL;
	}
	else return itone->second;
}

void Reactor::scanUnusedChannel( int freq )
{
	scanUnusedChannelFreq++;
	if(scanUnusedChannelFreq >= freq) {
		scanUnusedChannelFreq = 0;
		//2min未使用则尝试断开连接，并删除
		mChannelTable->scanUnusedChannel(120);
	}
}

void Reactor::shutdown()
{

}

void enableWrite(evutil_socket_t sock, short flags, void * args)
{
	Reactor* reactor = (Reactor*)args;
	std::string addr = reactor->findaddrbysock(sock);
	//向对应的socket发送 连接成功可以发送信号
	//对应的channel处在未连接状态，所以这个异步事件为判断connect成功
	//此处应删除可写事件，防止再次触发
	Channel_var mychannel = reactor->mChannelTable->getChannel(addr);
	if(!mychannel->IsRunOutOfTime(3)) {
		//成功连接上
		mychannel->setTimestamp();
		mychannel->setState(CHANNEL_CONNECTED);
		reactor->addReadable(mychannel->getSocket());
		std::cout << "catch writable" << std::endl;
		reactor->notifyChannel(sock);
	}
	else {
		//等待超时
		mychannel->setState(CHANNEL_ERROR);
		std::cout << "run out of time" << std::endl;
		reactor->notifyChannel(sock);
	}
}

void enableRead( evutil_socket_t sock, short flags, void * args )
{
	Reactor* reactor = (Reactor*)args;
	std::string addr = reactor->findaddrbysock(sock);
	Channel_var mychannel = reactor->mChannelTable->getChannel(addr);
	ByteBuffer_var readBuf = mychannel->getReadBuffer();
	printf("capacity %d, size %d\n", readBuf->Capacity(), readBuf->Size());
	int ret = 0;
	int offset = readBuf->Size();
	while((ret = recv(sock, readBuf->Inout() + offset, readBuf->Capacity(), 0)) > 0) {
		offset += ret;
	}
	ret = offset - readBuf->Size();
	readBuf->Size(offset);
	
	if(ret > 0)
	{
		mychannel->setTimestamp();
		while(1) {
			printf("sock %d, ret %d\n", sock, ret);
			int length = readBuf->getInt();
			int headerlength = readBuf->getInt();
			printf("length %d, headerlength %d\n", length, headerlength);
			readBuf->adjustReadpos(-8);
			if(!readBuf->isFullPackage(length+4)) {
				return (void)0;
			}

			readBuf->adjustReadpos(8);
			Byte_var headerdata = new Byte(headerlength+1);
			headerdata->setBuffer(readBuf->getBytes(headerlength), headerlength);
			RemotingCommand_var responseFuture = RemotingCommand::decode(headerdata->getBuffer());
			printf("header %s\n", headerdata->getBuffer());

			Byte_var bodydata = new Byte(length-4-headerlength);
			bodydata->setBuffer(readBuf->getBytes(length-4-headerlength), length-4-headerlength);
			
			responseFuture->setBody(Byte_var::Duplicate(bodydata));
			printf("body length %d\n", responseFuture->getBody()->size());
			
			reactor->setResponse(RemotingCommand_var::Duplicate(responseFuture));
			if(responseFuture->getCode() != 40) {
				reactor->notifyWorker(responseFuture->getOpaque());
			}
			else {
			}
			ret -= length+4;
			if(ret <= 0)
				break;
		}
		readBuf->Reset();
	}
	else
	{
		//连接断开
		printf("server closed\n");
	}
}

void Monitor(Reactor* reactor) {
	reactor->start();
}

void processTimeEclapsed( evutil_socket_t sock, short flags, void * args )
{
	Reactor* reactor = (Reactor*)args;
	printf("定时器!!!\n");
	reactor->scanLibeventContainer();
	reactor->scanUnusedChannel(20);
}
