#ifndef FINDBROKERRESULT_H
#define FINDBROKERRESULT_H

#include "version.h"

class FindBrokerResult {
private:
    std::string brokerAddr;
    bool slave;

public:
	FindBrokerResult(std::string& brokerAddr, bool slave) {
        this->brokerAddr = brokerAddr;
        this->slave = slave;
    }


    std::string& getBrokerAddr() {
        return brokerAddr;
    }


    bool isSlave() {
        return slave;
    }
};

#endif