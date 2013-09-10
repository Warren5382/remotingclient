#ifndef SENDRESULT_H
#define SENDRESULT_H
#include "version.h"
#include "common/mixall.h"
#include "common/message/messagequeue.h"

class SendResult : public LWPR::Object{
    private:
	std::string sendStatus;
    std::string msgId;
    MessageQueue* messageQueue;
    long queueOffset;


    public:
		SendResult() {};
	SendResult(std::string sendStatus, std::string msgId, MessageQueue* messageQueue, long queueOffset) {
        this->sendStatus = sendStatus;
        this->msgId = msgId;
        this->messageQueue = messageQueue;
        this->queueOffset = queueOffset;
    };


    std::string& getSendStatus() {
        return sendStatus;
    };


    std::string getMsgId() {
        return msgId;
    };


    MessageQueue* getMessageQueue() {
        return messageQueue;
    };


    long getQueueOffset() {
        return queueOffset;
    };
};

DECLAREVAR(SendResult);

#endif