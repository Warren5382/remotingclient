#ifndef JSONOBEJECTBASE_H
#define JSONOBEJECTBASE_H

#include <string>  
#include <vector>  
#include <list>
#include <set>
#include <boost/unordered_map.hpp>
#include "json/json.h"
using std::string;  
using std::vector;  
using std::list;
using std::set;


#define DoArrayDeSerialize_Wrapper(listType, type)\
	DoArrayDeSerialize<##listType<##type>, ##type>

#define DoSetDeSerialize_Wrapper(listType, type)\
	DoSetDeSerialize<##listType<##type>, ##type>

#define Type_Wrapper(listType, type)\
	(##listType<##type>*)

#define DoArraySerialize_Wrapper(listType, type)\
	DoArraySerialize(Type_Wrapper(listType, type)addr);

#define DoObjArrayDeSerialize_Wrapper(listType, type)\
	DoObjArrayDeSerialize<##listType<type>>

#define DoObjSetDeSerialize_Wrapper(listType, type)\
	DoObjSetDeSerialize<##listType<type>>

typedef void* (*GenerateJsonObjForDeSerializeFromOutSide)(const string& propertyName);

struct CJsonObjectBase  
{  
protected:  
	enum CEnumJsonTypeMap  
	{
		asBool = 1,  
		asInt,  
		asUInt,  
		asString,  
		asInt64,  
		asUInt64,  
		asJsonObj,
		asSpecialArray,
		asVectorArray,
		asListArray,
		asSetArray,
		asHashMap
	};  
public:  
	CJsonObjectBase(void){}  
public:  
	virtual ~CJsonObjectBase(void){		
	}  
	string Serialize()  
	{  
		Json::Value new_item = DoSerialize();         
		Json::FastWriter writer;    
		std::string out2 = writer.write(new_item);   
		return out2;  
	}  

	bool DeSerialize(const char* str)  
	{  
		Json::Reader reader;    
		Json::Value root;  
		if (reader.parse(str, root))  
		{    
			return DoDeSerialize(root);  
		}  
		return false;  
	}  


protected:
	Json::Value SerializeMap(void* pAddr)
	{
		Json::Value new_item;
		boost::unordered_map<std::string, std::string>* field = (boost::unordered_map<std::string, std::string>*)pAddr;
		boost::unordered_map<std::string, std::string>::iterator begin = field->begin();
		boost::unordered_map<std::string, std::string>::iterator end = field->end();
		for(; begin != end; begin++) {
			new_item[begin->first] = begin->second;
		}
		return new_item;
	}

	Json::Value DoSerialize()  
	{  
		Json::Value new_item;  
		size_t nSize = m_vectorName.size();  
		for (int i=0; i < nSize; ++i )  
		{  
			void* pAddr = m_vectorPropertyAddr[i];  
			switch(m_vectorType[i])  
			{  
			case asVectorArray:
			case asSetArray:
			case asListArray:
				new_item[m_vectorName[i]] = DoArraySerialize(pAddr, m_vectorType[i], m_vectorListParamType[i]);
				break;
			case asSpecialArray:  
				new_item[m_vectorName[i]] = DoSpecialArraySerialize(m_vectorName[i]); 
				break;  
			case asJsonObj:  
				new_item[m_vectorName[i]] = ((CJsonObjectBase*)pAddr)->DoSerialize();  
				break;  
			case asBool:  
				new_item[m_vectorName[i]] = Serialize<bool>(pAddr); 
			case asInt:  
				new_item[m_vectorName[i]] = Serialize<int>(pAddr); 
				break;  
			case asUInt:  
				new_item[m_vectorName[i]] = Serialize<unsigned int>(pAddr);  
				break;  
			case asInt64:  
				new_item[m_vectorName[i]] = Serialize<long long>(pAddr);  
				break;  
			case asUInt64:  
				new_item[m_vectorName[i]] = Serialize<unsigned long long>(pAddr);  
				break;  
			case asString:  
				new_item[m_vectorName[i]] = Serialize<string>(pAddr);
				break;
			case asHashMap:
				new_item[m_vectorName[i]] = SerializeMap(pAddr);
				break;
			default:  
				//我暂时只支持这几种类型，需要的可以自行添加
				break;  
			}         
		}  
		return new_item;  
	}
	bool DeSerializeMap(Json::Value& root, void* pAddr) {
		boost::unordered_map<std::string, std::string>* field = (boost::unordered_map<std::string, std::string>*)pAddr;
		Json::Value::Members member;//Members是vector<string>，typedef了而已
		member = root.getMemberNames();
		for (Json::Value::Members::iterator iter = member.begin(); iter != member.end(); iter++)
		{
			field->insert(boost::unordered_map<std::string, std::string>::value_type((*iter), (root)[(*iter)].asString()));
		}
		return true;
	}

	bool DoDeSerialize(Json::Value& root)  
	{  
		size_t nSize = m_vectorName.size();  
		for (int i=0; i < nSize; ++i )  
		{  
			void* pAddr = m_vectorPropertyAddr[i];  

			switch(m_vectorType[i])  
			{  
			case asListArray:
			case asVectorArray:
				{
					if (root.isNull() || root[m_vectorName[i]].isNull())
					{
						break;
					}
					DoArrayDeSerialize(m_vectorName[i], pAddr, root[m_vectorName[i]], m_vectorType[i], m_vectorListParamType[i]);
				}
				break;
			case asJsonObj:  
				{  
					if (!root[ m_vectorName[i] ].isNull())   
						((CJsonObjectBase*)pAddr)->DoDeSerialize(root[m_vectorName[i]]);  
				}                 
				break;  
			case asBool:  
				(*(bool*)pAddr) = root.get(m_vectorName[i], 0).asBool();  
				break;  
			case asInt:  
				(*(int*)pAddr) = root.get(m_vectorName[i], 0).asInt();  
				break;  
			case asUInt:  
				(*(unsigned int*)pAddr) = root.get(m_vectorName[i], 0).asUInt();  
				break;  
			case asInt64:  
				(*(long long*)pAddr) = root.get(m_vectorName[i], 0).asInt64();  
				break;  
			case asUInt64:  
				(*(unsigned long long*)pAddr) = root.get(m_vectorName[i], 0).asUInt64();  
				break;  
			case asString:  
				(*(string*)pAddr) = root.get(m_vectorName[i], "").asString(); 
				break;
			case asHashMap:
				DeSerializeMap(root[m_vectorName[i]], pAddr);
			default:  
				//我暂时只支持这几种类型，需要的可以自行添加   
				break;  
			}             
		}  
		return true;  
	}  

	void SetProperty(string name, CEnumJsonTypeMap type, void* addr,  CEnumJsonTypeMap listParamType = asInt)  
	{  
		m_vectorName.push_back(name);  
		m_vectorPropertyAddr.push_back(addr);  
		m_vectorType.push_back(type);  
		m_vectorListParamType.push_back(listParamType);
	}  
	virtual void SetPropertys() = 0;  
	//有特殊对象需要序列化时，请重载此函数 
	virtual Json::Value DoSpecialArraySerialize(const string& propertyName){ return "";}

	//在反序列化时，如果对象中嵌套了列表，并且列表中容纳的内容是其他特殊对象时，需要重载此函数，new出真正的对象
	virtual CJsonObjectBase* GenerateJsonObjForDeSerialize(const string& propertyName){ return NULL;}

	bool DoArrayDeSerialize(const string& propertyName, void* addr, Json::Value& root, CEnumJsonTypeMap listType, CEnumJsonTypeMap paramType)
	{
		if (listType == asVectorArray)
		{
			switch(paramType)
			{ 
			case asJsonObj:  
				{  
					return DoObjArrayDeSerialize_Wrapper(vector, CJsonObjectBase*)(propertyName, addr, root);
				}                 
				break;  
			case asBool:  
				//无 
				break; 
			case asInt:  
				return DoArrayDeSerialize_Wrapper(vector, int)(addr, root); 
			case asUInt:  
				return DoArrayDeSerialize_Wrapper(vector, unsigned int)(addr, root);   
			case asInt64:  
				return DoArrayDeSerialize_Wrapper(vector, long long)(addr, root);     
			case asUInt64:  
				return DoArrayDeSerialize_Wrapper(vector, unsigned long long)(addr, root);     
			case asString:  
				return DoArrayDeSerialize_Wrapper(vector, string)(addr, root);       
			default:
				break;  
			}
		}
		else if(listType == asListArray)
		{
			switch(paramType)
			{ 
			case asJsonObj:  
				{
					return DoObjArrayDeSerialize_Wrapper(list, CJsonObjectBase*)(propertyName, addr, root);
				}                 
				break;  
			case asBool:  
				return DoArrayDeSerialize_Wrapper(list, bool)(addr, root);
			case asInt:  
				return DoArrayDeSerialize_Wrapper(list, int)(addr, root); 
			case asUInt:  
				return DoArrayDeSerialize_Wrapper(list, unsigned int)(addr, root);   
			case asInt64:  
				return DoArrayDeSerialize_Wrapper(list, long long)(addr, root);     
			case asUInt64:  
				return DoArrayDeSerialize_Wrapper(list, unsigned long long)(addr, root);     
			case asString:  
				return DoArrayDeSerialize_Wrapper(list, string)(addr, root);     
			default:
				break;  
			}
		}
		else if(listType == asSetArray)
		{
			switch(paramType)
			{ 
			case asJsonObj:  
				{
					return DoObjSetDeSerialize_Wrapper(set, CJsonObjectBase*)(propertyName, addr, root, false);
				}                 
				break;  
			case asBool:  
				return DoSetDeSerialize_Wrapper(set, bool)(addr, root);
			case asInt:  
				return DoSetDeSerialize_Wrapper(set, int)(addr, root); 
			case asUInt:  
				return DoSetDeSerialize_Wrapper(set, unsigned int)(addr, root);   
			case asInt64:  
				return DoSetDeSerialize_Wrapper(set, long long)(addr, root);     
			case asUInt64:  
				return DoSetDeSerialize_Wrapper(set, unsigned long long)(addr, root);     
			case asString:  
				return DoSetDeSerialize_Wrapper(set, string)(addr, root);     
			default:
				break;  
			}
		}

		return true;
	}

	//特殊对象列表的反序列化
	template<typename T1>
	bool DoObjArrayDeSerialize(const string& propertyName, void* addr, Json::Value& node)
	{
		if (!node.isArray())
		{
			return false;
		}
		T1* pList = (T1*)addr;
		int size = node.size();
		for(int i = 0; i < size; ++i)
		{
			CJsonObjectBase* pNode = GenerateJsonObjForDeSerialize(propertyName);
			pNode->DoDeSerialize(node[i]);
			pList->push_back(pNode);
		}
		return true;
	}
	//对于set类型的特殊对象反序列化
	template<typename T1>
	bool DoObjSetDeSerialize(const string& propertyName, void* addr, Json::Value& node)
	{
		if (!node.isArray())
		{
			return false;
		}
		T1* pList = (T1*)addr;
		int size = node.size();
		for(int i = 0; i < size; ++i)
		{
			CJsonObjectBase* pNode = GenerateJsonObjForDeSerialize(propertyName);
			pNode->DoDeSerialize(node[i]);
			pList->insert(pNode);
		}
		return true;
	}
public:
	//常见类型列表的反序列化 
	template <typename T1, typename T2>
	static bool DoArrayDeSerialize(void* addr, Json::Value& node)
	{
		if (!node.isArray())
		{
			return false;
		}
		T1* pList = (T1*)addr;
		int size = node.size();
		for(int i = 0; i < size; ++i) {
			pList->push_back( DeSerialize<T2>(node[i]) );
		}
		return true;
	}
	//常见类型set的反序列化 
	template <typename T1, typename T2>
	static bool DoSetDeSerialize(void* addr, Json::Value& node)
	{
		if (!node.isArray())
		{
			return false;
		}
		T1* pList = (T1*)addr;
		int size = node.size();
		for(int i = 0; i < size; ++i) {
			pList->insert( DeSerialize<T2>(node[i]) );
		}
		return true;
	}

	//特殊对象列表的反序列化
	template<typename T1>
	static bool DoObjArrayDeSerialize(const string& propertyName, void* addr, Json::Value& node, GenerateJsonObjForDeSerializeFromOutSide funGenerate)
	{
		if (!node.isArray())
		{
			return false;
		}
		T1* pList = (T1*)addr;
		int size = node.size();
		for(int i = 0; i < size; ++i)
		{
			CJsonObjectBase* pNode = (CJsonObjectBase*)funGenerate(propertyName);
			pNode->DoDeSerialize(node[i]);
			pList->push_back(pNode);
		}
		return true;
	}
	//特殊对象set的反序列化
	template<typename T1>
	static bool DoObjSetDeSerialize(const string& propertyName, void* addr, Json::Value& node, GenerateJsonObjForDeSerializeFromOutSide funGenerate)
	{
		if (!node.isArray())
		{
			return false;
		}
		T1* pList = (T1*)addr;
		int size = node.size();
		for(int i = 0; i < size; ++i)
		{
			CJsonObjectBase* pNode = (CJsonObjectBase*)funGenerate(propertyName);
			pNode->DoDeSerialize(node[i]);
			pList->insert(pNode);
		}
		return true;
	}
protected:
	Json::Value DoArraySerialize(void* addr, CEnumJsonTypeMap listType, CEnumJsonTypeMap paramType)
	{
		if (listType == asVectorArray)
		{
			switch(paramType)
			{ 
			case asBool:  
				return "";
			case asJsonObj:
				return DoArraySerialize_Wrapper(vector, CJsonObjectBase*);
			case asInt:  
				return DoArraySerialize_Wrapper(vector, int);
			case asUInt:  
				return DoArraySerialize_Wrapper(vector, unsigned int); 
			case asInt64:  
				return DoArraySerialize_Wrapper(vector, long long);   
			case asUInt64:  
				return DoArraySerialize_Wrapper(vector, unsigned long long);  
			case asString:  
				return DoArraySerialize_Wrapper(vector, string);
			}
		}
		else if(listType == asListArray)
		{
			switch(paramType)
			{ 
			case asBool:  
				return DoArraySerialize_Wrapper(list, bool);
			case asJsonObj:
				return DoArraySerialize_Wrapper(list, CJsonObjectBase*);
			case asInt:  
				return DoArraySerialize_Wrapper(list, int);
			case asUInt:  
				return DoArraySerialize_Wrapper(list, unsigned int); 
			case asInt64:  
				return DoArraySerialize_Wrapper(list, long long);   
			case asUInt64:  
				return DoArraySerialize_Wrapper(list, unsigned long long);  
			case asString:  
				return DoArraySerialize_Wrapper(list, string);
			}
		}
		else if(listType == asSetArray)
		{
			switch(paramType)
			{ 
			case asBool:  
				return DoArraySerialize_Wrapper(set, bool);
			case asJsonObj:
				return DoArraySerialize_Wrapper(set, CJsonObjectBase*);
			case asInt:  
				return DoArraySerialize_Wrapper(set, int);
			case asUInt:  
				return DoArraySerialize_Wrapper(set, unsigned int); 
			case asInt64:  
				return DoArraySerialize_Wrapper(set, long long);   
			case asUInt64:  
				return DoArraySerialize_Wrapper(set, unsigned long long);  
			case asString:  
				return DoArraySerialize_Wrapper(set, string);
			}
		}
		return "";
	}
public:
	template <typename T1>
	static Json::Value DoArraySerialize(T1* pList)
	{
		Json::Value arrayValue;
		arrayValue.append(Json::Value::null);
		arrayValue.clear();
		for (T1::iterator it = pList->begin(); it != pList->end(); ++ it)
		{
			arrayValue.append(*it);			
		}
		return arrayValue;
	}	

	template <>
	static Json::Value DoArraySerialize(vector<CJsonObjectBase*>* pList)
	{
		Json::Value arrayValue;
		arrayValue.append(Json::Value::null);
		arrayValue.clear();
		for (vector<CJsonObjectBase*>::iterator it = pList->begin(); it != pList->end(); ++ it)
		{
			arrayValue.append((*it)->DoSerialize());			
		}
		return arrayValue;
	}

	template <>
	static Json::Value DoArraySerialize(list<CJsonObjectBase*>* pList)
	{
		Json::Value arrayValue;
		arrayValue.append(Json::Value::null);
		arrayValue.clear();
		for (list<CJsonObjectBase*>::iterator it = pList->begin(); it != pList->end(); ++ it)
		{
			arrayValue.append((*it)->DoSerialize());			
		}
		return arrayValue;
	}

	template <>
	static Json::Value DoArraySerialize(set<CJsonObjectBase*>* pList)
	{
		Json::Value arrayValue;
		arrayValue.append(Json::Value::null);
		arrayValue.clear();
		for (set<CJsonObjectBase*>::iterator it = pList->begin(); it != pList->end(); ++ it)
		{
			arrayValue.append((*it)->DoSerialize());			
		}
		return arrayValue;
	}

	static string JsonValueToString(Json::Value& tvalue)
	{
		Json::FastWriter writer;    
		return writer.write(tvalue);
	}
private:
	template <typename T>
	T Serialize(void* addr)
	{
		return (*(T*)addr);
	}

	template <typename T>
	static T DeSerialize(Json::Value& root)
	{
		return GetData<T>(root);
	}

	template <typename T>
	static T GetData(Json::Value& root)
	{
		T temp;
		return temp;
	}

	template <>
	static bool GetData(Json::Value& root)
	{
		return root.asBool();
	}

	template <>
	static int GetData(Json::Value& root)
	{
		return root.asInt();
	}

	template <>
	static unsigned int GetData(Json::Value& root)
	{
		return root.asUInt();
	}
	template <>
	static long long GetData(Json::Value& root)
	{
		return root.asInt64();
	}
	template <>
	static unsigned long long GetData(Json::Value& root)
	{
		return root.asUInt64();
	}
	template <>
	static string GetData(Json::Value& root)
	{
		return root.asString();
	}

private:  
	vector<string> m_vectorName;  
	vector<void*>  m_vectorPropertyAddr;  
	vector<CEnumJsonTypeMap>     m_vectorType;
	vector<CEnumJsonTypeMap>     m_vectorListParamType;
};

#endif
