#ifndef REACTOR_H
#define REACTOR_H

#include "version.h"
#include "bytebuffer\bytebuffer.h"
#include "tools\clientevent.h"
#include "protocol\remotingcommand.h"
#include "channeltable\channeltable.h"
#include "tools\countdown.h"
/*
 * Reactor为整个client的异步处理器，通过在反应器上注册异步事件，此反应器可以异步通知注册者
 */
class Reactor {
	public:
		Reactor(ChannelTable_ptr);
		~Reactor();

		void start();
		void shutdown();
		void RegisterLibevent(ClientEvent_ptr);
		void RegisterWorkerNotify(int workID);
		// 扫描libevent的队列，将其中的事件注册到base中
		void scanLibeventContainer();
		void addReadable(evutil_socket_t);
		void notifyChannel(evutil_socket_t);
		void notifyWorker(int);

		struct event_base* getBaseEvent();

		//client调用
		void waitChannelNotify(std::string);
		bool waitWorkerNotify(int);

		//返回response
		void setResponse(RemotingCommand_ptr res);
		RemotingCommand_ptr getResponse();

		// 与remotingclient共享channeltable
		ChannelTable_var mChannelTable;

		std::string findaddrbysock(evutil_socket_t);

		void scanUnusedChannel(int freq);
	private:
		// 用于注册libevent事件，由于libevnet单线程，所以先放到队列里，然后再取出注册
		std::queue<ClientEvent_ptr> LibeventContainer;
		// 针对socket的通知器，主要用于异步connect的通知
		boost::unordered_map<std::string, CountDown_ptr> ChannelNotifier;
		// 针对每个工作线程同步等待server数据结果的通知器
		boost::unordered_map<int, CountDown_ptr> WorkerNotifier;
		// channel和socketid的map，用于通过sock找到对应的channel
		boost::unordered_map<evutil_socket_t, std::string> SocktoChannel;
		// libevent 基本事件
		struct event_base* mBase;
		//
		void addWritable(struct event* ec);

		RemotingCommand_var mResponse;

		int scanUnusedChannelFreq;
};

/*
 *监控线程，负责监听来自server的数据，并分发给work线程
 */
void Monitor(Reactor*);

/*
 * Monitor线程处理可写事件
 */
void enableWrite(evutil_socket_t sock, short flags, void * args);

/*
 * Monitor线程处理可读事件
 */
void enableRead(evutil_socket_t sock, short flags, void * args);

/*
 * 处理超时事件
 */
void processTimeEclapsed(evutil_socket_t sock, short flags, void * args);

#endif
