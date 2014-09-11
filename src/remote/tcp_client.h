/*
 * tcp_client.h
 *
 *  Created on: Sep 9, 2014
 *      Author: DiamondS
 */

#ifndef TCP_CLIENT_H_
#define TCP_CLIENT_H_

#define TCP_CLIENT_SEND_SEC     5
#define TCP_CLIENT_SEND_USEC    0
#define TCP_CLIENT_RECV_SEC     15
#define TCP_CLIENT_RECV_USEC    0

typedef struct _tcp_client
{
    int stream;
    int debug;
    struct sockaddr_in myAddr;
} tcp_client_t;

int TCP_CLIENT_Init(tcp_client_t *tcpClient, char *ip, int port, int debug);
int TCP_CLIENT_Connect(tcp_client_t *tcpClient);
int TCP_CLIENT_Close(tcp_client_t *tcpClient);
int TCP_CLIENT_Send(tcp_client_t *tcpClient, char *data, int dataLength);
int TCP_CLIENT_Receive(tcp_client_t *tcpClient, char *data, int dataLength);

#endif /* TCP_CLIENT_H_ */
