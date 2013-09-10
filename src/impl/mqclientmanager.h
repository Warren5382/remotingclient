#ifndef MQCLIENTMANAGER_H
#define MQCLIENTMANAGER_H

#include "factory/MQClientFactory.h"

class MQClientManager {
public:
	static MQClientManager* instance;
	static MQClientManager* getInstance() {
		if(instance == NULL) {
			instance = new MQClientManager();
		}
		return instance;
	};

	MQClientFactory* getAndCreateMQClientFactory() {
		std::string clientId = "adddf";
		boost::unordered_map<std::string, MQClientFactory*>::iterator it;
		it = factoryTable.find(clientId);
		if (it == factoryTable.end()) {
			MQClientFactory* factory = new MQClientFactory(factoryIndexGenerator.GetAndIncrement(), clientId);
			this->factoryTable[clientId] = factory;
			return factory;
		}
		return it->second;
	};

	void removeCientFactory(std::string clientId) {
		this->factoryTable.erase(clientId);
	};

private:
	LWPR::AtomicInteger factoryIndexGenerator;
	boost::unordered_map<std::string, MQClientFactory*> factoryTable;
	MQClientManager() : factoryIndexGenerator(0) {};
};

#endif