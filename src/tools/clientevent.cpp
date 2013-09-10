#include "version.h"
#include "clientevent.h"

ClientEvent::ClientEvent() {
	this->event_write = NULL;
	this->event_read = NULL;
	this->mSock = -1;
}

ClientEvent::~ClientEvent() {
}


void ClientEvent::createReadandWrite() {

};

void ClientEvent::setSocket(evutil_socket_t sock) {
	mSock = sock;
}

evutil_socket_t ClientEvent::getSocket() {
	return mSock;
}

void ClientEvent::setWorkerID(int id) {
	mWorkerID = id;
};


void ClientEvent::setAddr(std::string id) {
	mAddr = id;
};


int ClientEvent::getWorkerID() {
	return mWorkerID;
};


std::string ClientEvent::getAddr() {
	return mAddr;
};

struct event* ClientEvent::getWriteEvent() {
	return event_write;
};


struct event* ClientEvent::getReadEvent() {
	return event_read;
};
