#include "communicationISD.h"




struct socketaddr_in Server::GetServerAddr()
{
	return stServerAddr;
}


struct socketaddr_in Server::GetClientAddr()
{
	return stServerAddr;
}


int Server::GetServerFd()
{
	return nServerFd;
}


int Server::GetClientFd()
{
	return nClientFd;
}


char* Server::GetRecvBuff()
{
	return RecvBuff;
}

char* Server::GetSendBuff()
{
	return SendBuff;
}




void Server::SetRecvBuff(char *_Recv)
{
	strcpy(RecvBuff, _Recv);
}

void Server::SetSendBuff(char* _Send)
{
	strcpy(SendBuff, _Send);
}




int Server::GenerateSock()
{
	nServerFd = socket(AF_INET, SOCK_STREAM, 0);
	if (nServerFd == -1)
	{
		printf("Server Socket Make Fail!!!!\n");
		return -1;
	}
}


int Server::BindSock()
{ 
	int nBindResult = bind(nServerFd, (struct sockaddr*)&stServerAddr, sizeof(stServerAddr));
	if (nBindResult == -1)
	{
		printf("Server Socket Bind Fail!!!!\n");
		close(nServerFd);
		return -1;
	}
}


int Server::ClientAccept()
{
	socklen_t nClientAddrSize = sizeof(stClientAddr);
	int nClientAcceptResult = accept(nServerFd, (sockaddr*) &nClientFd, &nClientAddrSize);
	if (nClientAcceptResult == -1)
	{
		printf("Clinet Accept Fail!!!!!\n");
		close(nClientFd);
		return -1;
	}

}


void *Server::Recv(void *vThis)
{
	Server* s = (Server*)vThis;

	int nClientFd_ = s->GetClientFd();
	char crRecvBuff_[RECV_BUFF_SIZE];

	
	while (1)
	{

		if (read(nClientFd_, crRecvBuff_, RECV_BUFF_SIZE) == -1)
		{
			printf("Recieve Fail!!!!!\n");
			break;
		}
		s->SetRecvBuff();
	}
	return 0;
}


void *Server::Send(void* vThis)
{
	Server* s = (Server*)vThis;

	int nClientFd_ = s->GetClientFd();

	char crSendBuff_[SEND_BUFF_SIZE];

	PacketProtocolApp *stCmd_;
	
	///////////
	while (1)
	{
		stCmd_ = CmdList.front();
		CmdList.pop_front();

		sprintf(crSendBuff_, "%s%s%s%s%s%s", stCmd_->crStart, stCmd_->crMode, stCmd_->crOpcode, 
			stCmd_->crLength, stCmd_->crData, stCmd_->crEnd);

		if (send(nClientFd_, crSendBuff_, SEND_BUFF_SIZE) == -1)
		{
			printf("Send Fail!!!!!\n");
			close(nClientFd_);
			break;
		}
	}
	return 0;
}



//1. send 쪽에만 리커넥트 기능 적용하여 StartComm() 함수 하나로 구현
//2. send 함수에서 에러가 나면 send, recv 쓰래드 종료 후 StartComm 다시 호출
//3. StartComm은 호출되면 bind, listen, accept 진행후 send, recv 쓰래드 다시 구동
int Server::StartComm()
{
	while (1)
	{
		int nOptValue = 1;

		if (GenerateSock() == -1)
		{
			continue;
		}

		setsockopt(nServerFd, SOL_SOCKET, SO_REUSEADDR, &nOptValue, sizeof(nOptValue));

		memset(&stServerAddr, 0, sizeof(stServerAddr));
		stServerAddr.sin_family = AF_INET;
		stServerAddr.sin_addr.s_addr = INADDR_ANY;
		stServerAddr.sin_port = htons(nServerPort);

		if (BindSock() == -1)
		{
			continue;
		}

		int nListenResult = listen(nServerFd, 5);
		if (nListenResult == -1)
		{
			continue;
		}

		while (1)
		{
			if (ClientAccept() == -1)
			{
				continue;
			}
		}

	}
	return 0;
}







