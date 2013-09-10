#ifndef CHANNEL_H
#define CHANNEL_H

#include "version.h"
#include "bytebuffer\bytebuffer.h"
/*
 *��Ӧһ��Channel������״̬
 */
enum ChannelState {
	CHANNEL_IDLE,
	CHANNEL_CONNECTED,
	CHANNEL_CLOSED,
	CHANNEL_ERROR
};

/*
 * socket�豸�������ķ�װ
 */
class Channel : public LWPR::Object{
	public:
		Channel();
		~Channel();

		// ����socket
		void Start(std::string);
		// �ر�
		void Shutdown();
		// ��������
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
		//�豸������
		evutil_socket_t sock;
		//ͨ��״̬
		ChannelState st;
		//�����������ڷ�������ʱ��ȫ
		boost::mutex sockmutex;
		// ��ַ
		std::string addr;
		// ʱ���
		time_t lastusedtime;
		// ��������
		ByteBuffer_var readBuffer;

		evutil_socket_t makeTCPSocket();
};

DECLAREVAR(Channel);

#endif
