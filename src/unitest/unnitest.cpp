#include <Windows.h>
void testsocket(void) {
	int iClientSock;
	char buf[ 256 ];
	struct sockaddr_in ServerAddr;
	int numbytes;
	// struct hostent *he;
	WSADATA WSAData;


	if( WSAStartup( MAKEWORD( 1, 1 ), &WSAData ) )//初始化
	{
		printf( "initializationing error!\n" );
		WSACleanup( );
		exit( 0 );
	}

	if( ( iClientSock = socket( AF_INET, SOCK_STREAM, 0 ) ) == INVALID_SOCKET )
	{
		printf( "创建套接字失败!\n" );
		WSACleanup( );
		exit( 0 );
	}

	ServerAddr.sin_family = AF_INET;
	ServerAddr.sin_port = htons( 10911 );
	// ServerAddr.sin_addr = *( ( struct in_addr * )he->h_addr );
	ServerAddr.sin_addr.s_addr = inet_addr( "127.0.0.1" );//记得换IP
	memset( &( ServerAddr.sin_zero ), 0, sizeof( ServerAddr.sin_zero ) );

	if( connect( iClientSock, ( struct sockaddr * ) & ServerAddr, sizeof( struct sockaddr ) ) == -1 )
	{
		printf( "connect失败!" );
		WSACleanup( );
		exit( 0 );
	}
	printf("connect ok\n");

	RemotingCommand_var request = RemotingCommand::createRequestCommand(34,NULL);
	std::ifstream in("D:/CplusplusPro/RemotingClient/cpp/heartbeatdata.txt");
	std::istreambuf_iterator<char> beg(in), end;
	std::string headerData(beg, end);
	in.close();
	request->setBody(headerData);
	ByteBuffer_ptr toSendData = request->encode();
	int ret = send(iClientSock, toSendData->Inout(), toSendData->Size(),0);
	printf("%d\n", ret);

	while(1) {
		numbytes = recv( iClientSock, buf, 256, 0 );
		if( numbytes == -1 )
		{
			printf( "recv失败!" );
			WSACleanup( );
			exit( 0 );
		}
		printf("%d\n", numbytes);
		buf[ numbytes ] = '\0';
		printf( "Received: %s", buf+8);
		Sleep(3000);
	}
	closesocket( iClientSock );
	WSACleanup( );
};

class CSubTestStruct : public CJsonObejectBase
{
public:
	CSubTestStruct()
	{
		SubMsgID = 0;
		SetPropertys();
	}
	ULONGLONG SubMsgID;
	string SubMsgTitle;
protected:
	//子类需要实现此函数，并且将相应的映射关系进行设置 
	virtual void SetPropertys()
	{
		SetProperty("SubMsgID", asUInt64, &SubMsgID);
		SetProperty("SubMsgTitle", asString, &SubMsgTitle);
	}
};
class CTestStruct : public CJsonObejectBase
{
public:
	CTestStruct()
	{
		subObj = new CSubTestStruct();
		SetPropertys();

	}
	ULONGLONG MsgID;
	string MsgTitle;
	string MsgContent;
	CSubTestStruct* subObj;
protected:
	//子类需要实现此函数，并且将相应的映射关系进行设置 
	virtual void SetPropertys()
	{
		SetProperty("MsgID", asUInt64, &MsgID);
		SetProperty("MsgTitle", asString, &MsgTitle);
		SetProperty("MsgContent", asString, &MsgContent);
		SetProperty("subObj", asJsonObj, subObj);
	}
};

class testMap : public CJsonObejectBase
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
		SetProperty("exfields", asHashmap, &exfields);
	}
};

void getMaxOffset(void* args) {
	RemotingClient* myclient = (RemotingClientImplement*)args;
	GetMaxOffsetRequestHeader* commandheader = new GetMaxOffsetRequestHeader();
	commandheader->setTopic("test");
	commandheader->setQueueId(11);
	RemotingCommand_var testcommand = RemotingCommand::createRequestCommand(30, commandheader);
	RemotingCommand_var response = myclient->invokeSync("127.0.0.1", testcommand, 3);
	std::cout << "opaque" << response->getOpaque() << std::endl;
};


/* 测试复杂对象序列化与反序列化
CTestStruct stru;
stru.MsgID = 11223344;
stru.MsgTitle = "黑黑";
stru.MsgContent = "哈哈";
std::cout <<stru.Serialize();

CTestStruct test2;
test2.DeSerialize(stru.Serialize().c_str());
std::cout << test2.MsgContent << std::endl;
*/

/* 测试map对象序列化与反序列化
testMap test;
test.exfields["hh"] = "bad";
test.exfields["zz"] = "helo";
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
