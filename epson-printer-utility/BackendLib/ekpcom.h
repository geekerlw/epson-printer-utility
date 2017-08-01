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
#ifndef __EKPCOM_H__
#define __EKPCOM_H__

enum _ERROR_PACKET_NUMBERS
{
	NOERR = 0,
	UNKNOWN_PACKET,
	PRINTER_DOWN,
	SYSTEM_HUNG = 16
};

int sock_open(void);
void sock_close(void);
int sock_write(char*, int*);
int sock_read(char*, int*);
int sock_reopen(void);
int sock_write_abandon_reply(char*, int*);
int sock_write_with_reply(char* buf, int* lp_wsize, char* rbuf, int* lp_rsize);

#endif /* __EKPCOM_H__ */
