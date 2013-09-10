#ifndef REACTOR_H
#define REACTOR_H

#include "version.h"
#include "bytebuffer\bytebuffer.h"
#include "tools\clientevent.h"
#include "protocol\remotingcommand.h"
#include "channeltable\channeltable.h"
#include "tools\countdown.h"
/*
 * ReactorΪ����client���첽��������ͨ���ڷ�Ӧ����ע���첽�¼����˷�Ӧ�������첽֪ͨע����
 */
class Reactor {
	public:
		Reactor(ChannelTable_ptr);
		~Reactor();

		void start();
		void shutdown();
		void RegisterLibevent(ClientEvent_ptr);
		void RegisterWorkerNotify(int workID);
		// ɨ��libevent�Ķ��У������е��¼�ע�ᵽbase��
		void scanLibeventContainer();
		void addReadable(evutil_socket_t);
		void notifyChannel(evutil_socket_t);
		void notifyWorker(int);

		struct event_base* getBaseEvent();

		//client����
		void waitChannelNotify(std::string);
		bool waitWorkerNotify(int);

		//����response
		void setResponse(RemotingCommand_ptr res);
		RemotingCommand_ptr getResponse();

		// ��remotingclient����channeltable
		ChannelTable_var mChannelTable;

		std::string findaddrbysock(evutil_socket_t);

		void scanUnusedChannel(int freq);
	private:
		// ����ע��libevent�¼�������libevnet���̣߳������ȷŵ������Ȼ����ȡ��ע��
		std::queue<ClientEvent_ptr> LibeventContainer;
		// ���socket��֪ͨ������Ҫ�����첽connect��֪ͨ
		boost::unordered_map<std::string, CountDown_ptr> ChannelNotifier;
		// ���ÿ�������߳�ͬ���ȴ�server���ݽ����֪ͨ��
		boost::unordered_map<int, CountDown_ptr> WorkerNotifier;
		// channel��socketid��map������ͨ��sock�ҵ���Ӧ��channel
		boost::unordered_map<evutil_socket_t, std::string> SocktoChannel;
		// libevent �����¼�
		struct event_base* mBase;
		//
		void addWritable(struct event* ec);

		RemotingCommand_var mResponse;

		int scanUnusedChannelFreq;
};

/*
 *����̣߳������������server�����ݣ����ַ���work�߳�
 */
void Monitor(Reactor*);

/*
 * Monitor�̴߳����д�¼�
 */
void enableWrite(evutil_socket_t sock, short flags, void * args);

/*
 * Monitor�̴߳���ɶ��¼�
 */
void enableRead(evutil_socket_t sock, short flags, void * args);

/*
 * ����ʱ�¼�
 */
void processTimeEclapsed(evutil_socket_t sock, short flags, void * args);

#endif
