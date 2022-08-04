#ifndef SOCKET_CLIENT_H
#define SOCKET_CLIENT_H

#include <stdio.h>
#include <string.h> //strlen
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>    //close
#include <arpa/inet.h> //close
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/time.h>
#include <errno.h>

#define TRUE 1
#define FALSE 0
#define MAX 100
#define PORT 10091
#define SA struct sockaddr

int configureSocket();
void listenSocket(int sockfd);
void bindSocket(int sockfd, struct sockaddr_in servaddr);
void connectSocket(int sockfd, struct sockaddr_in servaddr);

#endif