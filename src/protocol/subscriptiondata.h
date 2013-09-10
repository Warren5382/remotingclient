#ifndef SUBSCRIPTIONDATA_H
#define SUBSCRIPTIONDATA_H

#include "version.h"
class SubscriptionData : public CJsonObjectBase{
	private:
		std::string topic;
		std::string subString;
		std::set<std::string> tagsSet;
		std::set<int> codeSet;
		long subVersion;
	public:
		static std::string SUB_ALL;
		SubscriptionData() {
			SetPropertys();
		};
		SubscriptionData(std::string& topic, std::string& subString) {
			this->topic = topic;
			this->subString = subString;
			SetPropertys();
		};
		std::string& getTopic() {
			return topic;
		};
		void setTopic(std::string& topic) {
			this->topic = topic;
		};
		std::string& getSubString() {
			return subString;
		};
		void setSubString(std::string&subString) {
			this->subString = subString;
		};
		std::set<std::string>& getTagsSet() {
			return tagsSet;
		};
		void setTagsSet(std::set<std::string>& tagsSet) {
			this->tagsSet = tagsSet;
		};
		long getSubVersion() {
			return subVersion;
		};
		void setSubVersion(long subVersion) {
			this->subVersion = subVersion;
		};
		std::set<int>& getCodeSet() {
			return codeSet;
		};
		void setCodeSet(std::set<int>& codeSet) {
			this->codeSet = codeSet;
		};
protected:
	virtual void SetPropertys() {
		SetProperty("topic", asString, &topic);
		SetProperty("subString", asString, &subString);
		SetProperty("tagsSet", asVectorArray, &tagsSet);
		SetProperty("codeSet", asVectorArray, &codeSet);
		SetProperty("subVersion", asUInt64, &subVersion);
	};
};

#endif
