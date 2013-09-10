#ifndef OFFSETSTORE_H
#define OFFSETSTORE_H

class MessageQueue;

class OffsetStore {
public:
    virtual void load();
    virtual void updateOffset(MessageQueue* mq, long offset, bool increaseOnly);
    virtual long readOffset(MessageQueue* mq, bool fromStore);
	virtual void persistAll(std::set<MessageQueue*>& mqs);
    virtual void persist(MessageQueue* mq);
};

#endif