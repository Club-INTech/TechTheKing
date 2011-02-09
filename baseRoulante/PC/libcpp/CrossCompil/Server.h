/* A simple server in the internet domain using TCP
   The port number is passed as an argument */
#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>

class Socket{
	public:
		Socket(int port);
		void onOpen();
		void onSetPort();
		void onWrite(string msg);
		void onRead();
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
