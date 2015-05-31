#include "tcpCommunication.h"

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
	Data* data = new Data();
	tcp_client->Recv(data);
	printf("%s",data->getString().c_str());

	return 0;
}
int tcpDestroy()
{
	tcp_client->close();
	delete tcp_client;

	return 0;
}