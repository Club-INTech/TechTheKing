#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include "Obstacles.h"

class Socket{
	public:
		Socket(int port);
		void onOpen();
		void onSetPort();
		void onWrite(string msg);
		Obstacle* onRead();
		void onClose();
		~Socket();
	private:
		int m_sockfd;
		int m_newsockfd;
		int m_port;
		socklen_t m_cliLen;
		char m_buffer[256];
		struct sockaddr_in m_servAddr;
		struct sockaddr_in m_cliAddr;
};

#endif
