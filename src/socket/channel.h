#ifndef CHANNEL_H
#define CHANNEL_H

#include "version.h"
#include "bytebuffer\bytebuffer.h"
/*
 *对应一个Channel的连接状态
 */
enum ChannelState {
	CHANNEL_IDLE,
	CHANNEL_CONNECTED,
	CHANNEL_CLOSED,
	CHANNEL_ERROR
};

/*
 * socket设备描述符的封装
 */
class Channel : public LWPR::Object{
	public:
		Channel();
		~Channel();

		// 创建socket
		void Start(std::string);
		// 关闭
		void Shutdown();
		// 发送数据
		void SendData(char*, int);

		//
		std::string getAddr();
		//
		evutil_socket_t getSocket();
		//
		ChannelState getState();
		void setState(ChannelState);
		//
		void setTimestamp();
		bool IsRunOutOfTime(int threshold);
		//
		ByteBuffer_ptr getReadBuffer();

	private:
		//设备描述符
		evutil_socket_t sock;
		//通道状态
		ChannelState st;
		//互斥锁，用于发送数据时安全
		boost::mutex sockmutex;
		// 地址
		std::string addr;
		// 时间戳
		time_t lastusedtime;
		// 读缓冲区
		ByteBuffer_var readBuffer;

		evutil_socket_t makeTCPSocket();
};

DECLAREVAR(Channel);

#endif
