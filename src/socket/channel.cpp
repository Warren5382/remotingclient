#include "socket/channel.h"

Channel::Channel() : sock(-1), st(CHANNEL_IDLE), addr("127.0.0.1")
{
	time(&lastusedtime);
	readBuffer = new ByteBuffer(10240);
}

Channel::~Channel()
{

}

void Channel::Start(std::string addr)
{
	this->sock = makeTCPSocket();
	this->addr = addr;
	struct sockaddr_in serverAddr;

	serverAddr.sin_family = AF_INET;
	// 提取IP地址和Port号
	size_t pos = this->addr.find(':');
	std::string Portstr = this->addr.substr(pos+1);
	std::string Ipstr = this->addr.substr(0, this->addr.length()-Portstr.length()-1);
	int Portint = atoi(Portstr.c_str());
	serverAddr.sin_port = htons(Portint);
#ifdef WIN32
	serverAddr.sin_addr.S_un.S_addr = inet_addr(Ipstr.c_str());
#else
	serverAddr.sin_addr.S_addr = inet_addr(Ipstr.c_str());
#endif
	memset(serverAddr.sin_zero, 0x00, 8);

	connect(this->sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
}

void Channel::Shutdown()
{
	closesocket(sock);
	WSACleanup();
	ByteBuffer::Release(readBuffer);
}

void Channel::SendData(char* data, int length)
{
	sockmutex.lock();
	int ret = send(sock, data, length, 0);
	printf("sockfd %d, sendret %d, length %d, data %s\n", sock, ret, length, data+8);
	sockmutex.unlock();
}

evutil_socket_t Channel::makeTCPSocket()
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
#endif
	int on = 1;
	evutil_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	evutil_make_socket_nonblocking(sock);
#ifdef WIN32
	setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (const char*)&on, sizeof(on));
#else
	setsockopt(sock, SOL_SOCKET, TCP_NODELAY, (void*)&on, sizeof(on));
#endif
	return sock;
}

std::string Channel::getAddr() {
	return this->addr;
}

evutil_socket_t Channel::getSocket() {
	return this->sock;
}

ChannelState Channel::getState() {
	return this->st;
}

void Channel::setState(ChannelState st) {
	this->st = st;
}

void Channel::setTimestamp()
{
	time(&lastusedtime);
}

bool Channel::IsRunOutOfTime( int threshold )
{
	time_t nowtime;
	time(&nowtime);
	return (nowtime - lastusedtime > threshold);
}

ByteBuffer_ptr Channel::getReadBuffer()
{
	return ByteBuffer_var::Duplicate(readBuffer);
}
