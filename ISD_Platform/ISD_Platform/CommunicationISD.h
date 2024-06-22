#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <iostream>
#include <pthread.h>
#include <errno.h>
#include <cstdlib>
#include <cstring>

#include "DefineISD.h"


using namespace std;




class Server
{
private :
	int nServerFd;
	int nClientFd;

	struct socketaddr_in stServerAddr;
	struct socketaddr_in stClientAddr;

	char crServerIp[20];
	int nServerPort;

	int nClientAddrSize;

	char RecvBuff[RECV_BUFF_SIZE];
	char SendBuff[SEND_BUFF_SIZE];


public :
	Server();


	struct socketaddr_in GetServerAddr();
	struct socketaddr_in GetClientAddr();
	int GetServerFd();
	int GetClientFd();
	char *GetRecvBuff();
	char *GetSendBuff();

	void SetRecvBuff(char* _Recv);
	void SetSendBuff(char* _Send);
	
	int StartComm();
	int GenerateSock();
	int BindSock();
	int ClientAccept();
	void *Recv(void *vThis);
	void *Send(void* vThis)

	
};