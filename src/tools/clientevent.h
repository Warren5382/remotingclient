#ifndef CLIENTEVENT_H
#define CLIENTEVENT_H

#include "version.h"

enum ASYNCEVENTEXCEPTION {
	NEWEVENT_ERROR,
	ADDEVENT_ERROR
};

class ClientEvent : public LWPR::Object{
	public:
		/*
		 * ���캯��
		 */
		ClientEvent();

		/*
		 * ��������
		 */
		~ClientEvent();

		/*
		 * ������Ӧ��client�Ŀɶ�����д�¼�
		 */
		void createReadandWrite();

		/* 
		 * ��ȡsocket
		 */
		evutil_socket_t getSocket();

		/*
		 * ����socket
		 */
		void setSocket(evutil_socket_t sock);

		/*
		 * ���ù����߳�ID
		 */
		void setWorkerID(int id);

		/*
		 * ���ø��첽�¼���Ӧ��server��ַ
		 */
		void setAddr(std::string id);

		/*
		 * ��ȡ�����߳�ID
		 */
		int getWorkerID();

		/*
		 * ��ȡ�첽�¼���Ӧ��server��ַ
		 */
		std::string getAddr();

		/*
		 *��ȡ���첽�¼����еĿ�д�¼�
		 */
		struct event* getWriteEvent();

		/*
		 *��ȡ���첽�¼����еĿɶ��¼�
		 */
		struct event* getReadEvent();
	private:
		int mWorkerID;
		std::string mAddr;
		evutil_socket_t mSock;
	public:
		//libevent ���ڼ��socket��д���ɶ��¼�
		struct event* event_write;
		struct event* event_read;
};

DECLAREVAR(ClientEvent);

#endif
