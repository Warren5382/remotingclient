#ifndef MESSAGEEXT_H
#define MESSAGEEXT_H
#include "Message.h"

class MessageExt : public Message {
	private:
		long long serialVersionUID;
		int queueId;
		int storeSize;
		long queueOffset;
		int sysFlag;
		long bornTimestamp;
		std::string bornHost;
		long storeTimestamp;
		std::string storeHost;
		std::string msgId;
		long commitLogOffset;
		int bodyCRC;
		int reconsumeTimes;
		long preparedTransactionOffset;
	public:
		MessageExt() : serialVersionUID(5720810158625748049L) {
		};
};
#endif
