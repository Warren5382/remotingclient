#include "version.h"

class CSubTestStruct : public CJsonObjectBase  
{ 
public:
	CSubTestStruct()  
	{  
		SubMsgID = 0;  
		SetPropertys();  
	}  

	unsigned long long SubMsgID;  
	string SubMsgTitle;  
protected:  
	//子类需要实现此函数，并且将相应的映射关系进行设置   
	virtual void SetPropertys()  
	{  
		SetProperty("SubMsgID", asUInt64, &SubMsgID);  
		SetProperty("SubMsgTitle", asString, &SubMsgTitle);  
	}  
};  
class CTestStruct : public CJsonObjectBase  
{ 
public:
	CTestStruct()  
	{  
		SetPropertys();  
	}  
	~CTestStruct()
	{
		for (vector<CSubTestStruct*>::iterator it = testListSpecial.begin(); it != testListSpecial.end(); ++ it)
		{
			delete (*it);
		}
	}
	unsigned long long MsgID;  
	string MsgTitle;  
	string MsgContent;  
	CSubTestStruct subObj;  
	vector<int> intList;
	list<string> testList;
	vector<CSubTestStruct*> testListSpecial;
protected:  
	CJsonObjectBase* GenerateJsonObjForDeSerialize(const string& propertyName)
	{
		if("testListSpecial" == propertyName)
		{
			return new CSubTestStruct();
		}
		return NULL;
	}
	//子类需要实现此函数，并且将相应的映射关系进行设置   
	virtual void SetPropertys()  
	{
		SetProperty("MsgID", asUInt64, &MsgID);  
		SetProperty("MsgTitle", asString, &MsgTitle);  
		SetProperty("MsgContent", asString, &MsgContent);  
		SetProperty("subObj", asJsonObj, &subObj);
		SetProperty("intList", asVectorArray, &intList);
		SetProperty("testList", asListArray, &testList, asString);
		SetProperty("testListSpecial", asVectorArray, &testListSpecial, asJsonObj);		
	} 	
};

class testMap : public CJsonObjectBase
{
public:
	boost::unordered_map<std::string, std::string> exfields;
	std::string mName;
	int count;
public:
	testMap() {
		mName = "hello world";
		count = 11;
		SetPropertys();
	}
protected:
	virtual void SetPropertys()
	{
		SetProperty("mName", asString, &mName);
		SetProperty("count", asInt, &count);
		SetProperty("exfields", asHashMap, &exfields);
	}
};


int main(void) {
	CTestStruct stru;
	stru.MsgID = 11223344;
	stru.MsgTitle = "黑黑";
	stru.MsgContent = "哈哈";
	stru.testList.push_back("aaaa");
	stru.testList.push_back("bbbb");
	stru.intList.push_back(3333);
	stru.intList.push_back(3344);
	CSubTestStruct* obj = new CSubTestStruct();
	obj->SubMsgID = 2;
	obj->SubMsgTitle = "333";
	//stru.testListSpecial.push_back(obj);
	std::cout << stru.Serialize() << std::endl;
	/*
	testMap test;
	test.exfields["hh"] = "bad";
	test.exfields["zz"] = "hello";
	std::cout << test.Serialize();

	testMap test2;
	test2.mName = "good";
	test2.count = 12;
	std::cout << test2.exfields.size() << std::endl;
	test2.DeSerialize(test.Serialize().c_str());
	std::cout << test2.exfields["hh"] << " , " << test2.mName << std::endl;
	boost::unordered_map<std::string, std::string>::iterator it = test2.exfields.begin();
	for(; it != test2.exfields.end(); it++) {
		std::cout << it->first << it->second << std::endl;
	}
	*/
	int a;
	std::cin >> a;
	return 0;
}
