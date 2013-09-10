#ifndef MQPROTOS_H
#define MQPROTOS_H

enum MQRequestCode {
	// Broker ������Ϣ
	SEND_MESSAGE = 10,
	// Broker ������Ϣ
	PULL_MESSAGE = 11,
	// Broker ��ѯ��Ϣ
	QUERY_MESSAGE = 12,
	// Broker ��ѯBroker Offset
	QUERY_BROKER_OFFSET = 13,
	// Broker ��ѯConsumer Offset
	QUERY_CONSUMER_OFFSET = 14,
	// Broker ����Consumer Offset
	UPDATE_CONSUMER_OFFSET = 15,
	// Broker ���»�������һ��Topic
	UPDATE_AND_CREATE_TOPIC = 17,
	// Broker ɾ��һ��Topic����������������
	DELETE_TOPIC = 19,
	// Broker ��ȡ����Topic�����ã�Slave��Namesrv������Master��������ã�
	GET_ALL_TOPIC_CONFIG = 21,
	// Broker ��ȡ����Topic���ã�Slave��Namesrv������Master��������ã�
	GET_TOPIC_CONFIG_LIST = 22,
	// Broker ��ȡ����Topic�����б�
	GET_TOPIC_NAME_LIST = 23,
	// Broker Slave��ȡMaster����Consumer���ѽ���
	PULL_ALL_CONSUMER_OFFSET = 24,
	// Broker ����Broker�ϵ�����
	UPDATE_BROKER_CONFIG = 25,
	// Broker ��ȡBroker�ϵ�����
	GET_BROKER_CONFIG = 26,
	// Broker ����Brokerɾ���ļ�
	TRIGGER_DELETE_FILES = 27,
	// Broker ��ȡBroker����ʱ��Ϣ
	GET_BROKER_RUNTIME_INFO = 28,
	// Broker ����ʱ���ѯ���е�Offset
	SEARCH_OFFSET_BY_TIMESTAMP = 29,
	// Broker ��ѯ�������Offset
	GET_MAX_OFFSET = 30,
	// Broker ��ѯ������СOffset
	GET_MIN_OFFSET = 31,
	// Broker ��ѯ����������Ϣ��Ӧʱ��
	GET_EARLIEST_MSG_STORETIME = 32,
	// Broker ������ϢID����ѯ��Ϣ
	VIEW_MESSAGE_BY_ID = 33,
	// Broker Client��Client������������ע������
	HEART_BEAT = 34,
	// Broker Clientע��
	UNREGISTER_CLIENT = 35,
	// Broker Consumer�������˵���Ϣ���ط�����
	CONSUMER_SEND_MSG_BACK = 36,
	// Broker Commit����Rollback����
	END_TRANSACTION = 37,
	// Broker ��ȡConsumerId�б�ͨ��GroupName
	GET_CONSUMER_LIST_BY_GROUP = 38,
	// Broker ������Producer�ز�����״̬
	CHECK_TRANSACTION_STATE = 39,
	// Broker Broker֪ͨConsumer�б�仯
	NOTIFY_CONSUMER_IDS_CHANGED = 40,
	// Broker Consumer��Master��������
	LOCK_BATCH_MQ = 41,
	// Broker Consumer��Master��������
	UNLOCK_BATCH_MQ = 42,
	// Broker ��ȡ����Consumer Offset
	GET_ALL_CONSUMER_OFFSET = 43,
	// Broker ��ȡ���ж�ʱ����
	GET_ALL_DELAY_OFFSET = 45,
	// Namesrv ��Namesrv׷��KV����
	PUT_KV_CONFIG = 100,
	// Namesrv ��Namesrv��ȡKV����
	GET_KV_CONFIG = 101,
	// Namesrv ��Namesrv��ȡKV����
	DELETE_KV_CONFIG = 102,
	// Namesrv ע��һ��Broker�����ݶ��ǳ־û��ģ���������򸲸�����
	REGISTER_BROKER = 103,
	// Namesrv ж��һ��Broker�����ݶ��ǳ־û���
	UNREGISTER_BROKER = 104,
	// Namesrv ����Topic��ȡBroker Name��������(������������д����)
	GET_ROUTEINTO_BY_TOPIC = 105,
	// Namesrv ��ȡע�ᵽName Server������Broker��Ⱥ��Ϣ
	GET_BROKER_CLUSTER_INFO = 106,

	// Broker ���»�������һ��������
	UPDATE_AND_CREATE_SUBSCRIPTIONGROUP = 200,
	GET_ALL_SUBSCRIPTIONGROUP_CONFIG = 201
};

enum MQResponseCode {
	// Broker ˢ�̳�ʱ
	FLUSH_DISK_TIMEOUT = 10,
	// Broker ͬ��˫д��Slave������
	SLAVE_NOT_AVAILABLE = 11,
	// Broker ͬ��˫д���ȴ�SlaveӦ��ʱ
	FLUSH_SLAVE_TIMEOUT = 12,
	// Broker ��Ϣ�Ƿ�
	MESSAGE_ILLEGAL = 13,
	// Broker, Namesrv ���񲻿��ã����������ڹرջ���Ȩ������
	SERVICE_NOT_AVAILABLE = 14,
	// Broker, Namesrv �汾�Ų�֧��
	VERSION_NOT_SUPPORTED = 15,
	// Broker, Namesrv ��Ȩ��ִ�д˲����������Ƿ����ա�������������
	NO_PERMISSION = 16,
	// Broker, Topic������
	TOPIC_NOT_EXIST = 17,
	// Broker, Topic�Ѿ����ڣ�����Topic
	TOPIC_EXIST_ALREADY = 18,
	// Broker ����Ϣδ�ҵ��������Offset�������Offset�����Offset�޶�Ӧ��Ϣ��
	PULL_NOT_FOUND = 19,
	// Broker ���ܱ����ˣ�������֪ͨ��
	PULL_RETRY_IMMEDIATELY = 20,
	// Broker ����Ϣ�����Offset���Ϸ���̫С��̫��
	PULL_OFFSET_MOVED = 21,
	// Broker ��ѯ��Ϣδ�ҵ�
	QUERY_NOT_FOUND = 22,
	// Broker ���Ĺ�ϵ����ʧ��
	SUBSCRIPTION_PARSE_FAILED = 23,
	// Broker ���Ĺ�ϵ������
	SUBSCRIPTION_NOT_EXIST = 24,
	// Broker ���Ĺ�ϵ�������µ�
	SUBSCRIPTION_NOT_LATEST = 25,
	// Broker �����鲻����
	SUBSCRIPTION_GROUP_NOT_EXIST = 26,
	// Producer ����Ӧ�ñ��ύ
	TRANSACTION_SHOULD_COMMIT = 200,
	// Producer ����Ӧ�ñ��ع�
	TRANSACTION_SHOULD_ROLLBACK = 201,
	// Producer ����״̬δ֪
	TRANSACTION_STATE_UNKNOW = 202,
	// Producer ProducerGroup����
	TRANSACTION_STATE_GROUP_WRONG = 203
};

enum ResponseCode {
	// �ɹ�
	SUCCESS,
	// ������δ�����쳣
	SYSTEM_ERROR,
	// �����̳߳�ӵ�£�ϵͳ��æ
	SYSTEM_BUSY,
	// ������벻֧��
	REQUEST_CODE_NOT_SUPPORTED
};

#endif
