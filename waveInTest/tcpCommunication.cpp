#include "tcpCommunication.h"
#include <sstream>
TcpClient* tcp_client;

int tcpIni(const char* ip, int port)
{
	tcp_client = new TcpClient();
	tcp_client->setAddr(ip, port);
	tcp_client->Connect();

	return 0;
}
DWORD WINAPI tcpCommunicationStart(LPVOID pParam)
{
	int* frames_per_packet = (int*)pParam;
	std::stringstream ostream;

	Data* data = new Data();
	while( tcp_client->Recv(data) )
	{
		ostream<<data->getString();
		ostream>>(*frames_per_packet);
		printf("%s,%d\n",data->getString().c_str(),*frames_per_packet);
	}

	return 0;
}
int tcpDestroy()
{
	tcp_client->close();
	delete tcp_client;

	return 0;
}