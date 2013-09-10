#include "channeltable.h"

ChannelTable::ChannelTable() {

}

ChannelTable::~ChannelTable() {
	boost::unordered_map<std::string, Channel_ptr>::iterator it = this->mChannelMap.begin();
	for(; it != this->mChannelMap.end(); it++) {
		Channel_ptr channel = it->second;
		Channel::Release(channel);
	}
	this->mChannelMap.clear();
}

bool ChannelTable::insertChannel(Channel_ptr channel) {
	//加锁写。多线程安全
	boost::mutex::scoped_lock lock(mTableMutex);
	mChannelMap[channel->getAddr()] = channel;
	return true;
}

Channel_ptr ChannelTable::getChannel(std::string addr) {
	boost::unordered_map<std::string, Channel_ptr>::iterator it;
	it = mChannelMap.find(addr);
	if(it == mChannelMap.end()) {
		return NULL;
	}
	else {
		return Channel_var::Duplicate(it->second);
	}
}

void ChannelTable::scanUnusedChannel( int threshold )
{
	boost::unordered_map<std::string, Channel_ptr>::iterator it;
	Channel_ptr mychannel;
	for(it = mChannelMap.begin(); it != mChannelMap.end(); it++) {
		mychannel = it->second;
		if(mychannel->IsRunOutOfTime(threshold)) {
			mychannel->Shutdown();
		}
	}
}