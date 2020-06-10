/**************************************************************************** **
 * Copyright (C) 2001-2020 Inhand Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : tipc_client.c
 *     FIRST CREATION DATE    : 2020/06/10
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/06/10
 *     FILE DESCRIPTION       : 虚拟机操作系统需要先加载tipc模块，
 *                            : 使用命令# modprobe tipc
** ************************************************************************** */

#include "tipc_client.h"

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
	struct sockaddr_tipc server_addr;
	char buf[40] = {Client_MSG};
	char buf_input[40];

	printf("****** TIPC client hello world program started ******\n\n");

	wait_for_server(SERVER_TYPE, SERVER_INST, 10000);

	sd = socket(AF_TIPC, SOCK_RDM, 0);

	server_addr.family = AF_TIPC;
	server_addr.addrtype = TIPC_ADDR_NAME;
	server_addr.addr.name.name.type = SERVER_TYPE;
	server_addr.addr.name.name.instance = SERVER_INST;
	server_addr.addr.name.domain = 0;

	if (0 > sendto(sd, buf, strlen(buf)+1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)))
	{
		perror("Client: failed to send");
		exit(1);
	}

	if (0 >= recv(sd, buf, sizeof(buf), 0))
	{
		perror("Client: unexpected response");
		exit(1);
	}

	printf("Client: received response: %s \n", buf);

	while (1)
	{
		printf("Input Message: ");
		scanf("%s", buf_input);
		if (0 > sendto(sd, buf_input, strlen(buf_input)+1, 0, (struct sockaddr*)&server_addr, sizeof(server_addr)))
		{
			perror("Client: failed to send");
			exit(1);
		}
		if (0 >= recv(sd, buf, sizeof(buf), 0))
		{
			perror("Client: unexpected response");
			exit(1);
		}
		printf("Client: received response: %s \n", buf);
		fflush(stdin);
	}

	printf("\n****** TIPC client hello program finished ******\n");

    exit(0);
}

/***************************************************************************************
 * Function Name : wait_for_server
 * Description   : wait messages from server
 * @param [in ]  : name_type
 *               : name_instance
 *               : wait
 * @param [out]  : NULL
 * @return       : NULL
 ***************************************************************************************/
void wait_for_server(__u32 name_type, __u32 name_instance, int wait)
{
	int sd;
	struct sockaddr_tipc topsrv;
	struct tipc_subscr subscr;
	struct tipc_event event;

	sd = socket(AF_TIPC, SOCK_SEQPACKET, 0);

	memset(&topsrv, 0, sizeof(topsrv));
	topsrv.family = AF_TIPC;
	topsrv.addrtype = TIPC_ADDR_NAME;
	topsrv.addr.name.name.type = TIPC_TOP_SRV;
	topsrv.addr.name.name.instance = TIPC_TOP_SRV;

	/* Connect to topology server */
	if (0 > connect(sd, (struct sockaddr *)&topsrv, sizeof(topsrv)))
	{
		perror("Client: failed to connect to topology server");
		exit(0);
	}

	subscr.seq.type = htonl(name_type);
	subscr.seq.lower = htonl(name_instance);
	subscr.seq.upper = htonl(name_instance);
	subscr.timeout = htonl(wait);
	subscr.filter = htonl(TIPC_SUB_SERVICE);

	if (send(sd, &subscr, sizeof(subscr), 0) != sizeof(subscr))
	{
		perror("Client: failed to send subscription");
		exit(1);
	}

	/* Now wait for the subscription to fire */
	if (recv(sd, &event, sizeof(event), 0) != sizeof(event))
	{
		perror("Client: failed to receive event");
		exit(1);
	}

	if (event.event != htonl(TIPC_PUBLISHED)) {
		printf("Client: server {%u,%u} not published within %u [s]\n",
				name_type, name_instance, wait/1000);
		exit(1);
	}

	close(sd);
}

