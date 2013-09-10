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
		 * 构造函数
		 */
		ClientEvent();

		/*
		 * 析构函数
		 */
		~ClientEvent();

		/*
		 * 创建对应该client的可读，可写事件
		 */
		void createReadandWrite();

		/* 
		 * 获取socket
		 */
		evutil_socket_t getSocket();

		/*
		 * 设置socket
		 */
		void setSocket(evutil_socket_t sock);

		/*
		 * 设置工作线程ID
		 */
		void setWorkerID(int id);

		/*
		 * 设置该异步事件对应的server地址
		 */
		void setAddr(std::string id);

		/*
		 * 获取工作线程ID
		 */
		int getWorkerID();

		/*
		 * 获取异步事件对应的server地址
		 */
		std::string getAddr();

		/*
		 *获取该异步事件类中的可写事件
		 */
		struct event* getWriteEvent();

		/*
		 *获取该异步事件类中的可读事件
		 */
		struct event* getReadEvent();
	private:
		int mWorkerID;
		std::string mAddr;
		evutil_socket_t mSock;
	public:
		//libevent 用于检测socket可写，可读事件
		struct event* event_write;
		struct event* event_read;
};

DECLAREVAR(ClientEvent);

#endif
