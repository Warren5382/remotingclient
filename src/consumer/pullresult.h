#include "bytebuffer\bytebuffer.h"
#ifndef PULLRESULT_H
#define PULLRESULT_H
enum PullStatus {
	/**
	* �ҵ���Ϣ
	*/
	FOUND,
	/**
	* û���µ���Ϣ���Ա���ȡ
	*/
	NO_NEW_MSG,
	/**
	* �������˺�û��ƥ�����Ϣ
	*/
	NO_MATCHED_MSG,
	/**
	* Offset���Ϸ������ܹ�����߹�С
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