#pragma once
#include <list>

using namespace std;



#define PROT 5000
#define RECV_BUFF_SIZE 1038
#define SEND_BUFF_SIZE 1038


typedef struct PacketProtocolApp
{
	char crStart;
	char crMode;
	char crOpcode;
	char crLength[10];
	char crData[1024];
	char crEnd;
}PacketProtocolApp;
 


list<PacketProtocolApp*> CmdList;


