#ifndef REMOTINGCOMMAND_H
#define REMOTINGCOMMAND_H

#include "version.h"
#include "bytebuffer/bytebuffer.h"
#include "commandheader/commandcustomheader.h"

enum RemotingCommandType {
	REQUEST_COMMAND,
	RESPONSE_COMMAND
};

class RemotingCommand : virtual public LWPR::Object, virtual public CJsonObjectBase{
	public:
		std::string RemotingVersionKey;
	public:
		volatile int ConfigVersion;
		static LWPR::AtomicInteger* RequestId;
		int RPC_TYPE; //0, REQUEST_COMMAND; 1, RESPONSE_COMMAND
		int RPC_ONEWAY; //0, RPC; 1, Oneway

		/*
		 * Header 部分
		 */
		int code;
		std::string language;
		int version;
		int opaque;
		int flag;
		std::string remark;
		// 存储Header中的成员变量
		boost::unordered_map<std::string, std::string> extFields;

		CommandCustomHeader_var customHeader;
		/*
		 * Body部分
		 */
		Byte_var body;

	protected:
		RemotingCommand();
	public:
		static RemotingCommand* createRequestCommand(int code, CommandCustomHeader_ptr customHeader);
		static RemotingCommand* createResponseCommand(int code, std::string remark);
		~RemotingCommand();

		/*
		 * 设置类中需要序列化的成员变量
		 */
		virtual void SetPropertys();

		/*
		 * 只有通信层内部会调用，业务不会调用
		 */
		static RemotingCommand* createResponseCommand(int code, std::string remark, CommandCustomHeader_ptr classHeader);

		void makeCustomHeaderToNet();

	public:
		CommandCustomHeader_ptr getCustomHeader();
		void decodeCommandCustomHeader(CommandCustomHeader_ptr classHeader);

	public:
		void buildHeader(std::string &headerData);

	public:
		ByteBuffer_ptr encode();
		ByteBuffer_ptr encodeHeader();
		ByteBuffer_ptr encodeHeader(int bodyLength);

		static RemotingCommand* decode(char*);
		RemotingCommand* decode(ByteBuffer_ptr byteBuffer);

		void markResponseType();

		bool isResponseType();
		void markOnewayRPC();
		bool isOnewayRPC();
		int getCode();
		void setCode(int code);

		RemotingCommandType getType();
		std::string& getLanguage();
		void setLanguage(std::string language);
		int getVersion();
		void setVersion(int version);
		int getOpaque();
		void setOpaque(int opaque);
		int getFlag();
		void setFlag(int flag);
		std::string& getRemark();
		void setRemark(std::string remark);

		Byte_ptr getBody();
		void setBody(Byte_ptr body);

		std::string getExtFields();
		void setExtFields(std::string extFields);

		std::string toString();
};

DECLAREVAR(RemotingCommand);

#endif
