#include <event.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ECHO_PORT 8888
#define ECHO_SERVER "127.0.0.1"

struct echo_context {
	struct event_base* base;
	struct event* event_write;
	struct event* event_read;
	const char* echo_contents;
	int echo_contents_len;
	int recved;
};

void write_cb(evutil_socket_t sock, short flags, void* args)
{
	int error = 1;
	int len = sizeof(error);
	int ret = getsockopt(sock,SOL_SOCKET,SO_ERROR,(char*)&error,&len);
	printf("***************%d*************\n", ret);
	if(  ret < 0) {
		printf("wrong writable\n");  

	}
	else {
		struct echo_context* ec = (struct echo_context*)args;
		int ret = send(sock, ec->echo_contents, ec->echo_contents_len, 0);
		printf("conneted, write to echo server %d\n", ret);
		event_add(ec->event_read, 0);
	}
};

void read_cb(evutil_socket_t sock, short flags, void * args)
{
	struct echo_context *ec = (struct echo_context *)args; 
	char buf[128];
	int ret = recv(sock, buf, 128, 0);

	printf("read_cb, read %d bytes\n", ret);
	if(ret > 0)
	{
		ec->recved += ret;
		buf[ret] = 0;
		printf("recv:%s\n", buf);
	}
	else if(ret == 0)
	{
		printf("read_cb connection closed\n");
		event_base_loopexit(ec->base, NULL);
		return;
	}
	if(ec->recved < ec->echo_contents_len)
	{
		event_add(ec->event_read, 0);
	}
};

static evutil_socket_t make_tcp_socket()
{
#ifdef WIN32
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2,0), &wsaData);
#endif
	int on = 1;
	evutil_socket_t sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	evutil_make_socket_nonblocking(sock);
#ifdef WIN32
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (const char*)&on, sizeof(on));
#else
	setsockopt(sock, SOL_SOCKET, SO_KEEPALIVE, (void*)&on, sizeof(on));
#endif
	return sock;
};

static void echo_client(struct event_base *base)
{
	evutil_socket_t sock = make_tcp_socket();
	struct sockaddr_in serverAddr;
	struct event * ev_write = 0;
	struct event * ev_read = 0;
	struct timeval tv={10, 0};
	struct echo_context *ec = (struct echo_context*)calloc(1, sizeof(struct echo_context));

	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(ECHO_PORT);
#ifdef WIN32
	serverAddr.sin_addr.S_un.S_addr = inet_addr(ECHO_SERVER);
#else
	serverAddr.sin_addr.S_addr = inet_addr(ECHO_SERVER);
#endif
	memset(serverAddr.sin_zero, 0x00, 8);

	connect(sock, (struct sockaddr*)&serverAddr, sizeof(serverAddr));

	ev_write = event_new(base, sock, EV_WRITE, write_cb, (void*)ec);
	ev_read = event_new(base, sock, EV_READ , read_cb, (void*)ec);

	ec->event_write = ev_write;
	ec->event_read = ev_read;
	ec->base = base;
	ec->echo_contents = strdup("echo client tneilc ohce\n");
	ec->echo_contents_len = strlen(ec->echo_contents);
	ec->recved = 0;

	event_add(ev_write, &tv);
};

int main(void)
{
	struct event_base * base = 0;

	base = event_base_new();
	echo_client(base);
	event_base_dispatch(base);
	event_base_free(base);

	int a;
	scanf("%d",&a);

	return 0;
}