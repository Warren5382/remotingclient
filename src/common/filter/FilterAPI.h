#ifndef FILTERAPI_H
#define FILTERAPI_H
#include "version.h"
#include <vector>
#include "protocol\subscriptiondata.h"
namespace strtool
{
	string trim(const std::string& str)
	{
		std::string::size_type pos = str.find_first_not_of(' ');
		if (pos == std::string::npos)
		{
			return str;
		}
		std::string::size_type pos2 = str.find_last_not_of(' ');
		if (pos2 != std::string::npos)
		{
			return str.substr(pos, pos2 - pos + 1);
		}
		return str.substr(pos);
	}

	int split(const std::string& str, std::vector<string>& ret_, std::string sep = ",")
	{
		if (str.empty())
		{
			return 0;
		}

		std::string tmp;
		std::string::size_type pos_begin = str.find_first_not_of(sep);
		std::string::size_type comma_pos = 0;

		while (pos_begin != std::string::npos)
		{
			comma_pos = str.find(sep, pos_begin);
			if (comma_pos != std::string::npos)
			{
				tmp = str.substr(pos_begin, comma_pos - pos_begin);
				pos_begin = comma_pos + sep.length();
			}
			else
			{
				tmp = str.substr(pos_begin);
				pos_begin = comma_pos;
			}

			if (!tmp.empty())
			{
				ret_.push_back(tmp);
				tmp.clear();
			}
		}
		return 0;
	}

	std::string replace(const std::string& str, const std::string& src, const std::string& dest)
	{
		std::string ret;

		std::string::size_type pos_begin = 0;
		std::string::size_type pos       = str.find(src);
		while (pos != std::string::npos)
		{
			std::cout <<"replacexxx:" << pos_begin <<" " << pos <<"\n";
			ret.append(str.data() + pos_begin, pos - pos_begin);
			ret += dest;
			pos_begin = pos + 1;
			pos       = str.find(src, pos_begin);
		}
		if (pos_begin < str.length())
		{
			ret.append(str.begin() + pos_begin, str.end());
		}
		return ret;
	}

	int fnv1Hash(const char *key)
	{
		int hash = 0;
		for (const char *s = key; *s; s++)
			hash = 31*hash + (*s);
		return hash;
	};
};

class FilterAPI {
public:
	static SubscriptionData* buildSubscriptionData(std::string& topic, std::string& subString) {
		SubscriptionData* subscriptionData = new SubscriptionData();
		subscriptionData->setTopic(topic);
		subscriptionData->setSubString(subString);

		if (subString.empty() || subString == SubscriptionData::SUB_ALL) {
			subscriptionData->setSubString(SubscriptionData::SUB_ALL);
		}
		else {
			std::vector<std::string> tags;
			strtool::split(subString, tags, "\\|\\|");
			if (tags.size() > 0) {
				for (int i = 0; i < tags.size(); i++) {
					if (tags[i].length() > 0) {
						std::string trimString = strtool::trim(tags[i]);
						subscriptionData->getTagsSet().insert(trimString);
						subscriptionData->getCodeSet().insert(strtool::fnv1Hash(trimString.c_str()));
					}
				}
			}
		}

		return subscriptionData;
	}

};

#endif