#ifndef CHANNELTABLE_H
#define CHANNELTABLE_H

#include "version.h"
#include "socket/channel.h"

class ChannelTable : public LWPR::Object{
	private:
		//������ ����mapд����
		boost::mutex mTableMutex;
		//�洢socket���ӵ�������keyΪserver��address��valueΪ��Ӧ���׽���
		boost::unordered_map<std::string, Channel_ptr> mChannelMap;
	public:
		ChannelTable();
		~ChannelTable();
		//�����ߴ���һ��channel�����뵽������
		bool insertChannel(Channel_ptr);
		//����������server��ַ��ȡ�ö�Ӧ��channel
		Channel_ptr getChannel(std::string);
		//
		void scanUnusedChannel(int threshold);
};
DECLAREVAR(ChannelTable);
#endif
