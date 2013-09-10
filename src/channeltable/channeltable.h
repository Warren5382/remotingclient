#ifndef CHANNELTABLE_H
#define CHANNELTABLE_H

#include "version.h"
#include "socket/channel.h"

class ChannelTable : public LWPR::Object{
	private:
		//互斥锁 用于map写保护
		boost::mutex mTableMutex;
		//存储socket连接的容器，key为server的address，value为对应的套接字
		boost::unordered_map<std::string, Channel_ptr> mChannelMap;
	public:
		ChannelTable();
		~ChannelTable();
		//调用者创建一个channel，插入到容器中
		bool insertChannel(Channel_ptr);
		//调用者输入server地址，取得对应的channel
		Channel_ptr getChannel(std::string);
		//
		void scanUnusedChannel(int threshold);
};
DECLAREVAR(ChannelTable);
#endif
