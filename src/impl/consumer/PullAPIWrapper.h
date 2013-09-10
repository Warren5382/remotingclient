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
	* ����ȡ������д�����Ҫ����Ϣ�����л�
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
	* ÿ�����ж�Ӧ������Ӧ�ı�����������ĸ���������
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
				// TODO �˴����ܶ�Name Serverѹ��������Ҫ����
				this->mQClientFactory->updateTopicRouteInfoFromNameServer(mq->getTopic());
				findBrokerResult =
					this->mQClientFactory->findBrokerAddressInSubscribe(mq->getBrokerName(),
					this->recalculatePullFromWhichNode(mq), false);
			}

			if (findBrokerResult != NULL) {
				int sysFlagInner = sysFlag;

				// Slave������ʵʱ�ύ���ѽ��ȣ����Զ�ʱ�ύ
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