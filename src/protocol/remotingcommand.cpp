#include "remotingcommand.h"
#include <fstream>
LWPR::AtomicInteger* RemotingCommand::RequestId = new LWPR::AtomicInteger(0);
RemotingCommand::RemotingCommand() : RemotingVersionKey("rocketmq.remoing.version"), ConfigVersion(-1), 
RPC_TYPE(0), RPC_ONEWAY(1), language("JAVA"), version(0), flag(0) {
	this->opaque = RequestId->GetAndIncrement();
	this->body = new Byte();
	SetPropertys();
}

RemotingCommand* RemotingCommand::createRequestCommand(int code, CommandCustomHeader_ptr customHeader) {
	RemotingCommand_var cmd = new RemotingCommand();
	cmd->setCode(code);
	cmd->customHeader = customHeader;
	return RemotingCommand_var::Duplicate(cmd);
}

RemotingCommand* RemotingCommand::createResponseCommand(int code, std::string remark) {
	RemotingCommand_var cmd = new RemotingCommand();
	cmd->setRemark(remark);
	return RemotingCommand_var::Duplicate(cmd);
}

RemotingCommand* RemotingCommand::createResponseCommand(int code, std::string remark, CommandCustomHeader_ptr classHeader) {
	RemotingCommand_var cmd = new RemotingCommand();
	cmd->markResponseType();
	cmd->setCode(code);
	cmd->setRemark(remark);

	if (classHeader != NULL) {
		try {
			CommandCustomHeader_var objectHeader = new CommandCustomHeader(); //tudo
			//cmd->customHeader = objectHeader;//
		}
		catch (myException e) {
			return NULL;
		}
	}
	return cmd;
}

void RemotingCommand::makeCustomHeaderToNet() {
	//序列化业务包头
	this->customHeader->settoHashMap(this->extFields);
}

CommandCustomHeader_ptr RemotingCommand::getCustomHeader() {
	return CommandCustomHeader_var::Duplicate(customHeader);
}

void RemotingCommand::decodeCommandCustomHeader(CommandCustomHeader_ptr objectHeader) {
	if (!this->extFields.empty()) {	
		objectHeader->getfromHashMap(this->extFields);
	}
}

void RemotingCommand::buildHeader(std::string &headerData) {
	if(customHeader != NULL) {
		this->makeCustomHeaderToNet();
	}
	headerData = this->Serialize();
}

ByteBuffer_ptr RemotingCommand::encode() {
	int length = 4;
	int headerlength = 0;
	/*
	   std::ifstream in("D:/CplusplusPro/RemotingClient/cpp/Readme.txt");
	   std::istreambuf_iterator<char> beg(in), end;
	   std::string headerData(beg, end);
	 */
	std::string headerData = "";
	this->buildHeader(headerData);
	headerlength = headerData.length();
	length += headerlength;

	if (this->body->size() != 0) {
		length += this->body->size();
	}

	ByteBuffer_var result = new ByteBuffer(length+4);
	result->putInt(length);
	result->putInt(headerlength);
	result->putBytes(const_cast<char*>(headerData.c_str()), headerlength);

	if (this->body->size() != 0) {
		result->putBytes(this->body->getBuffer(), this->body->size());
	}
	return ByteBuffer_var::Duplicate(result);
}

ByteBuffer_ptr RemotingCommand::encodeHeader() {
	ByteBuffer_var bytebuffer = this->encodeHeader(this->body->size());
	return ByteBuffer_var::Duplicate(bytebuffer);
}

/*
 * 只打包Header, body部分独立传输
 */
ByteBuffer_ptr RemotingCommand::encodeHeader(int bodyLength) {
	int length = 4;
	int headerlength = 0;
	std::string headerData = "";
	this->buildHeader(headerData);
	headerlength = headerData.length();
	length += headerlength;
	length += bodyLength;

	ByteBuffer_var result = new ByteBuffer();
	result->putInt(length);
	result->putInt(headerlength);

	result->putBytes(const_cast<char*>(headerData.c_str()), headerlength);

	return ByteBuffer_var::Duplicate(result);
}

void RemotingCommand::SetPropertys()
{
	SetProperty("code", asInt, &code);
	SetProperty("language", asString, &language);
	SetProperty("version", asInt, &version);
	SetProperty("opaque", asInt, &opaque);
	SetProperty("flag", asInt, &flag);
	SetProperty("remark", asString, &remark);
	SetProperty("extFields", asHashMap, &extFields);
}

RemotingCommand* RemotingCommand::decode(char* content) {
	CommandCustomHeader_var header;
	RemotingCommand_var responseFuture = RemotingCommand::createResponseCommand(0, "hello");
	responseFuture->DeSerialize(content);
	return RemotingCommand_var::Duplicate(responseFuture);
}

RemotingCommand* RemotingCommand::decode(ByteBuffer_ptr byteBuffer) {
	RemotingCommand_var cmd = new RemotingCommand();
	return RemotingCommand_var::Duplicate(cmd);
}

void RemotingCommand::markResponseType() {
	int bits = 1 << this->RPC_TYPE;
	this->flag |= bits;
}

bool RemotingCommand::isResponseType() {
	int bits = 1 << this->RPC_TYPE;
	return (this->flag & bits) == bits;
}

void RemotingCommand::markOnewayRPC() {
	int bits = 1 << this->RPC_ONEWAY;
	this->flag |= bits;
}

bool RemotingCommand::isOnewayRPC() {
	int bits = 1 << this->RPC_ONEWAY;
	return (this->flag & bits) == bits;
}

int RemotingCommand::getCode() {
	return this->code;
}

void RemotingCommand::setCode(int code) {
	this->code = code;
}

RemotingCommandType RemotingCommand::getType() {
	if (this->isResponseType()) {
		return RESPONSE_COMMAND;
	}
	return REQUEST_COMMAND;
}

std::string& RemotingCommand::getLanguage() {
	return this->language;
}

void RemotingCommand::setLanguage(std::string language) {
	this->language = language;
}

int RemotingCommand::getVersion() {
	return this->version;
}

void RemotingCommand::setVersion(int version) {
	this->version = version;
}

int RemotingCommand::getOpaque() {
	return this->opaque;
}

void RemotingCommand::setOpaque(int opaque) {
	this->opaque = opaque;
}

int RemotingCommand::getFlag() {
	return this->flag;
}

void RemotingCommand::setFlag(int flag) {
	this->flag = flag;
}

std::string& RemotingCommand::getRemark() {
	return this->remark;
}

void RemotingCommand::setRemark(std::string remark) {
	this->remark = remark;
}

Byte_ptr RemotingCommand::getBody() {
	return Byte_var::Duplicate(this->body);
}

void RemotingCommand::setBody(Byte_ptr body) {
	this->body = body;
}

std::string RemotingCommand::getExtFields() {
	//return this->extFields;
}

void RemotingCommand::setExtFields(std::string extFields) {
	//this->extFields = extFields;
}

std::string RemotingCommand::toString() {
	return "to be continued";
}

RemotingCommand::~RemotingCommand()
{
	extFields.clear();
	CommandCustomHeader::Release(customHeader);
}