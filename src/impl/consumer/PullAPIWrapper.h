#ifndef PULLAPIWRAPPER_H
#define PULLAPIWRAPPER_H

#include "version.h"
#include "impl\findbrokerresult.h"
#include "commandheader\commandcustomheader.h"

class PullAPIWrapper {
private:
	boost::unordered_map<MessageQueue*, long/* brokerId */> pullFromWhichNodeTable;

	MQClientFactory* mQClientFactory;
	std::string consumerGroup;

public:
	PullAPIWrapper(MQClientFactory* mQClientFactory, std::string& consumerGroup) {
		this->mQClientFactory = mQClientFactory;
		this->consumerGroup = consumerGroup;
	}


	void updatePullFromWhichNode(MessageQueue* mq, long brokerId) {
		this->pullFromWhichNodeTable[mq]= brokerId;
	}

	/**
	* 对拉取结果进行处理，主要是消息反序列化
	*/
	PullResult_ptr processPullResult(MessageQueue* mq, PullResult_ptr pullResult,
		SubscriptionData* subscriptionData) {
			if(pullResult->getPullStatus() == PullStatus::FOUND) {
				ByteBuffer_var byteBuffer = new ByteBuffer(pullResult->getMessageBinary()->size());
				byteBuffer->putBytes(pullResult->getMessageBinary()->getBuffer(), pullResult->getMessageBinary()->size());
				byteBuffer->adjustReadpos(-pullResult->getMessageBinary()->size());
				printf("store size %d, %d\n", byteBuffer->getInt(), pullResult->getMessageBinary()->size());
				printf("%d,%d,%d,%d\n", byteBuffer->getInt(), byteBuffer->getInt(), byteBuffer->getInt(), byteBuffer->getInt());
			}
			return pullResult;
	}


	/**
	* 每个队列都应该有相应的变量来保存从哪个服务器拉
	*/
	long recalculatePullFromWhichNode(MessageQueue* mq) {
		boost::unordered_map<MessageQueue*, long>::iterator it = this->pullFromWhichNodeTable.find(mq);
		if(it != this->pullFromWhichNodeTable.end()) {
			return it->second;
		}
		return 0;
	}

	PullResult_ptr pullKernelImpl(//
		MessageQueue* mq,// 1
		std::string& subExpression,// 2
		long subVersion,// 3
		long offset,// 4
		int maxNums,// 5
		int sysFlag,// 6
		long commitOffset,// 7
		long brokerSuspendMaxTime,// 8
		long timeout,// 9
		std::string communicationMode,// 10
		bool pullCallback// 11
		) {
			FindBrokerResult* findBrokerResult =
				this->mQClientFactory->findBrokerAddressInSubscribe(mq->getBrokerName(),
				this->recalculatePullFromWhichNode(mq), false);
			if (NULL == findBrokerResult) {
				// TODO 此处可能对Name Server压力过大，需要调优
				this->mQClientFactory->updateTopicRouteInfoFromNameServer(mq->getTopic());
				findBrokerResult =
					this->mQClientFactory->findBrokerAddressInSubscribe(mq->getBrokerName(),
					this->recalculatePullFromWhichNode(mq), false);
			}

			if (findBrokerResult != NULL) {
				int sysFlagInner = sysFlag;

				// Slave不允许实时提交消费进度，可以定时提交
				/*
				if (findBrokerResult->isSlave()) {
					sysFlagInner = PullSysFlag.clearCommitOffsetFlag(sysFlagInner);
				}
				*/

				PullMessageRequestHeader_var requestHeader = new PullMessageRequestHeader();
				requestHeader->setConsumerGroup(this->consumerGroup);
				requestHeader->setTopic(mq->getTopic());
				requestHeader->setQueueId(mq->getQueueId());
				requestHeader->setQueueOffset(offset);
				requestHeader->setMaxMsgNums(maxNums);
				requestHeader->setSysFlag(sysFlagInner);
				requestHeader->setCommitOffset(commitOffset);
				requestHeader->setSuspendTimeoutMillis(brokerSuspendMaxTime);
				requestHeader->setSubscription(subExpression);
				requestHeader->setSubVersion(subVersion);

				PullResult_var pullResult 
				= this->mQClientFactory->getMQClientAPIImpl()->pullMessage(findBrokerResult->getBrokerAddr(), PullMessageRequestHeader_var::Duplicate(requestHeader), timeout);
				delete findBrokerResult;
				return PullResult_var::Duplicate(pullResult);
			}
			return NULL;
	}
};
#endif