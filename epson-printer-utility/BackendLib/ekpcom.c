/*
* Copyright (C) Seiko Epson Corporation 2014.
*
*  This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License as published by
* the Free Software Foundation; either version 2 of the License, or
* (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <sys/types.h>
//#include <sys/socket.h>
//#include <netinet/in.h>
//#include <unistd.h>
#include <winsock.h>
#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include "ekpcom.h"
#include "backend_api.h"
const char Packet_Key[] = "pcp";

#pragma comment(lib, "Ws2_32.lib")

#ifndef MSG_NOSIGNAL
#define MSG_NOSIGNAL 0x2000
#endif

enum _PACKET_DATA
{
	Key_Size = 3,
	Header_Size = 5,
	Accsess_Wait = 10,
	Max_Data_Size = 1024
};

static int server_sock_fd = -1;

int
sock_open(void)
{
	int sockfd, len;
	int port = DAEMON_PORT;
	struct sockaddr_in address;

	if (server_sock_fd >= 0)
		return 0;

	/* socket open */
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	address.sin_family = AF_INET;
	address.sin_addr.s_addr = htonl(INADDR_ANY);
	address.sin_port = htons(port);
	len = sizeof(address);
	if (connect(sockfd, (struct sockaddr *)&address, len))
	{
		return -1;
	}

	server_sock_fd = sockfd;
	return 0;
}

void
sock_close(void)
{
	if (server_sock_fd >= 0)
	{
		shutdown(server_sock_fd, 2);
		server_sock_fd = -1;
	}
	return;
}

int
sock_write(char* buf, int* lp_wsize)
{
	char packet[Max_Data_Size + Header_Size];
	int size;

	size = *lp_wsize;
	*lp_wsize = 0;

	if (server_sock_fd < 0)
		return -1;

	if (size <= 0)
		return 0;

	memcpy(packet, Packet_Key, Key_Size);
	packet[3] = size / 0xFF;
	packet[4] = size % 0xFF;
	memcpy(packet + Header_Size, buf, size);

	size = send(server_sock_fd, packet, size + Header_Size, MSG_NOSIGNAL);

	if (size < 0)
		return -1;

	*lp_wsize = size;
	return 0;
}

int
sock_read(char* buf, int* lp_rsize)
{
	char packet[Max_Data_Size + Header_Size];
	int size, dsize;

	size = *lp_rsize;
	*lp_rsize = 0;

	if (server_sock_fd < 0)
		return -1;

	if (size <= 0)
		return 0;

	dsize = recv(server_sock_fd, packet, size, MSG_NOSIGNAL);

	if (dsize < 0)
		return -1;
	else if (dsize < Header_Size)
		return 0;

	if (memcmp(packet, Packet_Key, Key_Size) != 0)
		return 0;

	dsize = packet[3];
	dsize <<= 8;
	dsize = dsize | (packet[4] & 0xff);

	if (dsize == 0)		/* Received a message of error */
		return packet[5];

	if (dsize > size)
		return 0;

	memcpy(buf, packet + Header_Size, dsize);

	*lp_rsize = dsize;
	return 0;
}

int
sock_reopen(void)
{
	sock_close();
	return sock_open();
}

int
sock_write_abandon_reply(char* buf, int* lp_wsize)
{
	char dummy[128];
	int len = 128;
	int tmp_size = *lp_wsize;

	if (sock_write(buf, lp_wsize) < 0)
	{
		sock_reopen();
		*lp_wsize = tmp_size;
		sock_write(buf, lp_wsize);
	}

	if (*lp_wsize <= 0)
		return 1;

	sock_read(dummy, &len);

	return 0;
}

int
sock_write_with_reply(char* buf, int* lp_wsize, char* rbuf, int* lp_rsize)
{

	int len = 256;
	int tmp_size = *lp_wsize;

	if (sock_write(buf, lp_wsize) < 0)
	{
		sock_reopen();
		*lp_wsize = tmp_size;
		sock_write(buf, lp_wsize);
	}

	if (*lp_wsize <= 0)
		return 1;

	sock_read(rbuf, &len);

	*lp_rsize = len;

	return 0;
}
