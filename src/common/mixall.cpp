#include "MixAll.h"

std::string MixAll::ROCKETMQ_HOME_ENV = "ROCKETMQ_HOME";
std::string MixAll::ROCKETMQ_HOME_PROPERTY = "rocketmq.home.dir";

std::string MixAll::NAMESRV_ADDR_ENV = "NAMESRV_ADDR";
std::string MixAll::NAMESRV_ADDR_PROPERTY = "rocketmq.namesrv.addr";

std::string MixAll::WS_DOMAIN_NAME = "jmenv.tbsite.net";
// 
std::string MixAll::WS_ADDR = "http://jmenv.tbsite.net:8080/rocketmq/nsaddr";
std::string MixAll::DEFAULT_TOPIC = "TBW102";
std::string MixAll::DEFAULT_PRODUCER_GROUP = "DEFAULT_PRODUCER";
std::string MixAll::DEFAULT_CONSUMER_GROUP = "DEFAULT_CONSUMER";
std::string MixAll::CLIENT_INNER_PRODUCER_GROUP = "CLIENT_INNER_PRODUCER";
std::string MixAll::SELF_TEST_TOPIC = "SELF_TEST_TOPIC";

//std::string MixAll::DEFAULT_CHARSET = "UTF-8";
std::string MixAll::MASTER_ID = "0";

std::string MixAll::RETRY_GROUP_TOPIC_PREFIX = "%RETRY%";
std::string MixAll::DLQ_GROUP_TOPIC_PREFIX = "%DLQ%";

std::string ConsumeType::CONSUME_ACTIVELY = "CONSUME_ACTIVELY";
std::string ConsumeType::CONSUME_PASSIVELY = "CONSUME_PASSIVELY";


std::string MessageModel::BROADCASTING = "BROADCASTING";
std::string MessageModel::CLUSTERING = "CLUSTERING";

std::string ConsumeFromWhere::CONSUME_FROM_LAST_OFFSET = "CONSUME_FROM_LAST_OFFSET";
std::string ConsumeFromWhere::CONSUME_FROM_LAST_OFFSET_AND_FROM_MIN_WHEN_BOOT_FIRST = "CONSUME_FROM_LAST_OFFSET_AND_FROM_MIN_WHEN_BOOT_FIRST";
std::string ConsumeFromWhere::CONSUME_FROM_MIN_OFFSET = "CONSUME_FROM_MIN_OFFSET";
std::string ConsumeFromWhere::CONSUME_FROM_MAX_OFFSET = "CONSUME_FROM_MAX_OFFSET";

std::string SendStatus::SEND_OK = "SEND_OK";
std::string SendStatus::FLUSH_DISK_TIMEOUT = "FLUSH_DISK_TIMEOUT";
std::string SendStatus::FLUSH_SLAVE_TIMEOUT = "FLUSH_SLAVE_TIMEOUT";
std::string SendStatus::SLAVE_NOT_AVAILABLE = "SLAVE_NOT_AVAILABLE";

std::string ServiceState::CREATE_JUST = "CREATE_JUST";
std::string ServiceState::RUNNING = "RUNNING";
std::string ServiceState::SHUTDOWN_ALREADY = "SHUTDOWN_ALREADY";

std::string CommunicationMode::SYNC = "SYNC";
std::string CommunicationMode::ASYNC = "ASYNC";
std::string CommunicationMode::ONEWAY = "ONEWAY";