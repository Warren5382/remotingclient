#ifndef MQPULLCONSUMER_H
#define MQPULLCONSUMER_H

#include "MQConsumer.h"
#include "PullResult.h"
#include "MessageQueueListener.h"

/**
 * 消费者，主动方式消费
 * 
 * @author shijia.wxr<vintage.wang@gmail.com>
 * @since 2013-7-24
 */
class MQPullConsumer : public MQConsumer {
    /**
     * 启动服务
     * 
     * @throws MQClientException
     */
    virtual void start() = 0;


    /**
     * 关闭服务
     */
    virtual void shutdown();


    /**
     * 注册监听队列变化的listener对象
     * 
     * @param topic
     * @param listener
     *            一旦发生变化，客户端会主动回调listener对象
     */
	virtual void registerMessageQueueListener(std::string& topic, MessageQueueListener listener) = 0;


    /**
     * 指定队列，主动拉取消息，即使没有消息，也立刻返回
     * 
     * @param mq
     *            指定具体要拉取的队列
     * @param subExpression
     *            订阅过滤表达式字符串，broker依据此表达式进行过滤。目前只支持或运�?br>
     *            eg: "tag1 || tag2 || tag3"<br>
     *            如果subExpression等于null或�?，则表示全部订阅
     * @param offset
     *            从指定队列哪个位置开始拉�?     * @param maxNums
     *            一次最多拉取条�?     * @return 参见PullResult
     * @throws MQClientException
     * @throws InterruptedException
     * @throws MQBrokerException
     * @throws RemotingException
     */
	virtual PullResult pull(MessageQueue* mq, std::string& subExpression, long offset, int maxNums) = 0;


    virtual void pull(MessageQueue* mq, std::string& subExpression, long offset, int maxNums, bool pullCallback) = 0;


    /**
     * 指定队列，主动拉取消息，如果没有消息，则broker阻塞一段时间再返回（时间可配置�?br>
     * broker阻塞期间，如果有消息，则立刻将消息返�?     * 
     * @param mq
     *            指定具体要拉取的队列
     * @param subExpression
     *            订阅过滤表达式字符串，broker依据此表达式进行过滤。目前只支持或运�?br>
     *            eg: "tag1 || tag2 || tag3"<br>
     *            如果subExpression等于null或�?，则表示全部订阅
     * @param offset
     *            从指定队列哪个位置开始拉�?     * @param maxNums
     *            一次最多拉取条�?     * @return 参见PullResult
     * @throws InterruptedException
     * @throws MQBrokerException
     * @throws RemotingException
     * @throws MQClientException
     */
    virtual PullResult pullBlockIfNotFound(MessageQueue* mq, std::string& subExpression, long offset, int maxNums)= 0;

    virtual void pullBlockIfNotFound(MessageQueue* mq, std::string& subExpression, long offset, int maxNums, bool pullCallback) = 0;

    /**
     * 更新消费进度<br>
     * 只是更新Consumer缓存中的数据，如果是广播模式，则定时更新到本地存�?br>
     * 如果是集群模式，则定时更新到远端Broker<br>
     * <p/>
     * P.S. 可频繁调用，无性能开销
     * 
     * @param mq
     * @param offset
     * @throws MQClientException
     */
    virtual void updateConsumeOffset(MessageQueue* mq, long offset) = 0;


    /**
     * 获取消费进度，返�?1表示出错
     * 
     * @param mq
     * @param fromStore
     * @return
     * @throws MQClientException
     */
    virtual long fetchConsumeOffset(MessageQueue* mq, bool fromStore) = 0;


    /**
     * 根据topic获取MessageQueue，以均衡方式在组内多个成员之间分�?     * 
     * @param topic
     *            消息Topic
     * @return 返回队列集合
     * @throws MQClientException
     */
	virtual std::set<MessageQueue*>& fetchMessageQueuesInBalance(std::string& topic) = 0;
};

#endif
