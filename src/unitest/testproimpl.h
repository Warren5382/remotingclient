#ifndef TESTPROIMPL_H
#define TESTPROIMPL_H

class DefaultMQProducerImpl;

class TestImpl {
public:
	DefaultMQProducerImpl* defaultmqproducerimpl;
	TestImpl() {};
	void test() {
		printf("OK\n");
	};
	~TestImpl() {};
};
#endif