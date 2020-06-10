/**************************************************************************** **
 * Copyright (C) 2001-2020 Inhand Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : tipc_server.c
 *     FIRST CREATION DATE    : 2020/06/10
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/06/10
 *     FILE DESCRIPTION       : 虚拟机操作系统需要先加载tipc模块，
 *                            : 使用命令# modprobe tipc
** ************************************************************************** */

#include "tipc_server.h"
#include "/home/xuke/bin/dbg.h"

/****************************************************************************
 *  Function Name : main
 *  Description   : The Main Function.
 *  Input(s)      : argc - The numbers of input value.
 *                : argv - The pointer to input specific parameters.
 *  Output(s)     : NULL
 *  Returns       : 0
 ****************************************************************************/
int main(int argc, const char *argv[])
{
	int sd;
	char inbuf[40];
	char outbuf[40] = Server_MSG;
	char outbuf2[40] = Server_RSP;
	struct sockaddr_tipc server_addr;
	struct sockaddr_tipc client_addr;
	socklen_t alen = sizeof(client_addr);

	printf("****** TIPC server hello world program started ******\n\n");

	server_addr.family = AF_TIPC;
	server_addr.addrtype = TIPC_ADDR_NAMESEQ;
	server_addr.addr.nameseq.type = SERVER_TYPE;
	server_addr.addr.nameseq.lower = SERVER_INST;
	server_addr.addr.nameseq.upper = SERVER_INST;
	server_addr.scope = TIPC_ZONE_SCOPE;

	sd = socket(AF_TIPC, SOCK_RDM, 0);

	if (0 != bind(sd, (struct sockaddr *)&server_addr, sizeof(server_addr)))
	{
		printf("Server: failed to bind port name!\n");
		perror("bind failed");
		exit(0);
	}

	if (0 >= recvfrom(sd, inbuf, sizeof(inbuf), 0, (struct sockaddr *)&client_addr, &alen))
	{
		perror("Server: unexpected message");
	}
	printf("Server: Message received:%s !\n", inbuf);
	
	if (0 > sendto(sd, outbuf, strlen(outbuf)+1, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)))
	{
		perror("Server: failed to send");
	}

	while (1)
	{
		if (0 >= recvfrom(sd, inbuf, sizeof(inbuf), 0, (struct sockaddr *)&client_addr, &alen))
		{
			perror("Server: unexpected message");
		}
		printf("Server: Message received:%s !\n", inbuf);

		strcat(outbuf2, inbuf);
		if (0 > sendto(sd, outbuf2, strlen(outbuf2)+1, 0, (struct sockaddr *)&client_addr, sizeof(client_addr)))
		{
			perror("Server: failed to send");
		}
		memset(outbuf2+sizeof(Server_RSP)-1, 0, 40-sizeof(Server_RSP)+1);
		/*my_debug_msg("%s", outbuf2);*/
	}

	printf("\n****** TIPC server hello program finished ******\n");

	exit(0);
}

