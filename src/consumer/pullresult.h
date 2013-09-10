#include "bytebuffer\bytebuffer.h"
#ifndef PULLRESULT_H
#define PULLRESULT_H
enum PullStatus {
	/**
	* 找到消息
	*/
	FOUND,
	/**
	* 没有新的消息可以被拉取
	*/
	NO_NEW_MSG,
	/**
	* 经过过滤后，没有匹配的消息
	*/
	NO_MATCHED_MSG,
	/**
	* Offset不合法，可能过大或者过小
	*/
	OFFSET_ILLEGAL
};
class PullResult : public LWPR::Object{
    private:
	PullStatus pullStatus;
    long nextBeginOffset;
    long minOffset;
    long maxOffset;
	long suggestWhichBrokerId;
	Byte_var messageBinary;


    public:
	PullResult() {};
	PullResult(PullStatus pullStatus, long nextBeginOffset, long minOffset, long maxOffset, long suggestWhichBrokerId, Byte_ptr messageBinary) {
        this->pullStatus = pullStatus;
        this->nextBeginOffset = nextBeginOffset;
        this->minOffset = minOffset;
        this->maxOffset = maxOffset;
		this->messageBinary = messageBinary;
		this->suggestWhichBrokerId = suggestWhichBrokerId;
    };
	
	~PullResult() {};


    PullStatus getPullStatus() {
        return pullStatus;
    };


    long getNextBeginOffset() {
        return nextBeginOffset;
    };


    long getMinOffset() {
        return minOffset;
    };


    long getMaxOffset() {
        return maxOffset;
    };

	long getSuggestWhichBrokerId() {
		return suggestWhichBrokerId;
	};

	Byte_ptr getMessageBinary() {
		return Byte_var::Duplicate(messageBinary);
	};
};

DECLAREVAR(PullResult);
#endif