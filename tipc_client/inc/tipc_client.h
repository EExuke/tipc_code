/**************************************************************************** **
 * Copyright (C) 2001-2020 Inhand Communications, Inc.
 **************************************************************************** **/

/* ************************************************************************** **
 *     MODULE NAME            : system
 *     LANGUAGE               : C
 *     TARGET ENVIRONMENT     : Any
 *     FILE NAME              : tipc_client.h
 *     FIRST CREATION DATE    : 2020/06/10
 * --------------------------------------------------------------------------
 *     Version                : 1.0
 *     Author                 : EExuke
 *     Last Change            : 2020/06/10
 *     FILE DESCRIPTION       : API exported to external moduels
** ************************************************************************** */
#ifndef _TIPC_CLIENT_H_
#define _TIPC_CLIENT_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <linux/tipc.h>

/*------------------Macro Definition-------------------------*/
#define SERVER_TYPE  18888
#define SERVER_INST  17

#define Client_MSG    "hello World"
/*------------------End of Macro Definition------------------*/

/*------------------API Definition---------------------------*/
void wait_for_server(__u32 name_type, __u32 name_instance, int wait);
/*------------------End of API Definition--------------------*/

#endif /* End of _TIPC_CLIENT_H_ */

