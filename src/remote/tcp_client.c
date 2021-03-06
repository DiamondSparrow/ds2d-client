/*
 * tcp_client.c
 *
 *  Created on: Sep 9, 2014
 *      Author: DiamondS
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/types.h>

#include "tcp_client.h"
#include "display.h"
#include "types.h"

int TCP_CLIENT_Init(tcp_client_t *tcpClient, char *ip, int port, int debug)
{
    struct timeval tv_send = {0};
    struct timeval tv_recv = {0};

    tv_send.tv_sec = TCP_CLIENT_SEND_SEC;   // Timeout in seconds
    tv_send.tv_usec = TCP_CLIENT_SEND_USEC; // Not init'ing this can cause strange errors
    tv_recv.tv_sec = TCP_CLIENT_RECV_SEC;   // Timeout in seconds
    tv_recv.tv_usec = TCP_CLIENT_RECV_USEC; // Not init'ing this can cause strange errors

    tcpClient->debug = debug;

    if ((tcpClient->stream = socket(PF_INET, SOCK_STREAM, 0)) == -1)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed to create socket. %s.\n", strerror(errno));
        return -1;
    }

    /*
    if (setsockopt(tcpClient->stream, SOL_SOCKET, SO_REUSEADDR, (int *) 1, sizeof(int)) == -1)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed to set socket option SO_REUSEADDR. %s.\n", strerror(errno));
        return -2;
    }
    */
    /*
    if(setsockopt(tcpClient->stream, SOL_SOCKET, SO_KEEPALIVE, (char *)1, sizeof(int)) == -1)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed to set socket option SO_KEEPALIVE. %s.\n", strerror(errno));
        return -3;
    }
    */
    if(setsockopt(tcpClient->stream, SOL_SOCKET, SO_RCVTIMEO, (char *) &tv_recv, sizeof(struct timeval)) == -1)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed to set socket option SO_RCVTIMEO. %s.\n", strerror(errno));
        return -4;
    }
    if(setsockopt(tcpClient->stream, SOL_SOCKET, SO_SNDTIMEO, (char *) &tv_send, sizeof(struct timeval)) == -1)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed to set socket option SO_SNDTIMEO. %s.\n", strerror(errno));
        return -5;
    }

    tcpClient->myAddr.sin_family = AF_INET;
    tcpClient->myAddr.sin_port = htons(port);

    memset(&(tcpClient->myAddr.sin_zero), 0, 8);
    tcpClient->myAddr.sin_addr.s_addr = inet_addr(ip);

    return 0;
}

int TCP_CLIENT_Connect(tcp_client_t *tcpClient)
{
    if (connect(tcpClient->stream, (struct sockaddr*) &tcpClient->myAddr, sizeof(tcpClient->myAddr)) == -1)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed connect to socket. %s.\n", strerror(errno));
        return -1;
    }

    return 0;
}

int TCP_CLIENT_Close(tcp_client_t *tcpClient)
{
    close(tcpClient->stream);

    return 0;
}

int TCP_CLIENT_Send(tcp_client_t *tcpClient, char *data, int dataLength)
{
    int ret = -1;

    if( (ret = send(tcpClient->stream, data, dataLength, 0)) != dataLength)
    {
        DISPLAY_Debug(TRUE, displayDebugError, "Failed to send data to socket. %s.\n", strerror(errno));
        return -1;
    }

    return ret;
}

int TCP_CLIENT_Receive(tcp_client_t *tcpClient, char *data, int dataLength)
{
    int ret = -1;

    if( (ret = recv(tcpClient->stream, data, dataLength, 0)) < 1)
    {
        if(errno == ECONNRESET)
        {
            DISPLAY_Debug(TRUE, displayDebugError, "Failed to receive data from socket (%d). %s.\n", errno, strerror(errno));
            return -2;
        }
        return -1;
    }

    return ret;
}
