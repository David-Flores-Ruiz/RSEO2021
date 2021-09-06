/*
 * Copyright (c) 2001-2003 Swedish Institute of Computer Science.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 3. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
 * SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
 * OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
 * IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
 * OF SUCH DAMAGE.
 *
 * This file is part of the lwIP TCP/IP stack.
 *
 * Author: Adam Dunkels <adam@sics.se>
 *
 */
#include "tcpecho.h"

#include "lwip/opt.h"

#if LWIP_NETCONN

#include "lwip/sys.h"
#include "lwip/api.h"

/* IP address SERVER = 192.168.0.103 */														// SERVER IP
#define serverIP_ADDR0 192																	// IP NET
#define serverIP_ADDR1 168																	// IP NET
#define serverIP_ADDR2 0																	// IP NET
#define serverIP_ADDR3 103																	// IP HOST

/*-----------------------------------------------------------------------------------*/
static void
tcpecho_thread(void *arg)
{
  struct netconn *conn, *newconn;
  err_t err;
  LWIP_UNUSED_ARG(arg);

  ip4_addr_t server_ipaddr;																	// SERVER IP
  IP4_ADDR(&server_ipaddr, serverIP_ADDR0, serverIP_ADDR1, serverIP_ADDR2, serverIP_ADDR3);	// SERVER IP

  /* Create a new connection identifier. */
  /* Bind connection to well known port number 7. */
#if LWIP_IPV6
  conn = netconn_new(NETCONN_TCP_IPV6);
  netconn_bind(conn, IP6_ADDR_ANY, 7);
#else /* LWIP_IPV6 */																// CLIENT
  conn = netconn_new(NETCONN_TCP);													// socket()
  //netconn_bind(conn, IP_ADDR_ANY, 7);										   		Delete bind()
#endif /* LWIP_IPV6 */
  LWIP_ERROR("tcpecho: invalid conn", (conn != NULL), return;);

  /* Tell connection to go into listening mode. */
  //netconn_listen(conn);													   		Delete listen()

  while (1) {

    /* Grab new connection. */
    //err = netconn_accept(conn, &newconn);											Delete accept()
    /*printf("accepted new connection %p\n", newconn);*/
    /* Process the new connection. */
//    if (err == ERR_OK) {

	  err = netconn_connect(conn, &server_ipaddr, 7);	  	  	  	  	  	  	  	// connect()

      struct netbuf *buf;
      void *data = 'a';
      u16_t len = 1;																// Specifies length

      err = netconn_write(conn, data, len, NETCONN_COPY);							// write()

      while ((err = netconn_recv(conn, &buf)) == ERR_OK) {
        /*printf("Recved\n");*/
        do {

             netbuf_data(buf, &data, &len);											// read()

#if 0
            if (err != ERR_OK) {
              printf("tcpecho: netconn_write: error \"%s\"\n", lwip_strerr(err));
            }
#endif
        } while (netbuf_next(buf) >= 0);
        netbuf_delete(buf);
      }
      /*printf("Got EOF, looping\n");*/
      /* Close connection and discard connection identifier. */
      netconn_close(conn);														// close()
      netconn_delete(conn);
//    } if
  }
}
/*-----------------------------------------------------------------------------------*/
void
tcpecho_init(void)
{
  sys_thread_new("tcpecho_thread", tcpecho_thread, NULL, DEFAULT_THREAD_STACKSIZE, DEFAULT_THREAD_PRIO);
}
/*-----------------------------------------------------------------------------------*/

#endif /* LWIP_NETCONN */
