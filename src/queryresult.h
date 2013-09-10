#ifndef QUERYRESULT_H
#define QUERYRESULT_H
#include "common/message/MessageExt.h"
/**
* ��ѯ��Ϣ���ؽ��
*/
class QueryResult {
private:
	long indexLastUpdateTimestamp;
	std::list<MessageExt> messageList;

public:
	QueryResult(long indexLastUpdateTimestamp, std::list<MessageExt>& messageList) {
		this->indexLastUpdateTimestamp = indexLastUpdateTimestamp;
		this->messageList = messageList;
	}


	long getIndexLastUpdateTimestamp() {
		return indexLastUpdateTimestamp;
	}


	std::list<MessageExt>& getMessageList() {
		return messageList;
	}
};

#endif