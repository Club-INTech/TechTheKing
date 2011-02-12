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
#include "Thread.h"
#define TAILLE_BUFFER 256
	
class Socket : public Thread{
	public:
		static void creer(int port);
		static Socket* instance();
		void SetPort();
		~Socket();
	private:
		void thread();
		Obstacle* trouverObstacle();
		Socket(int port);
		void onOpen();
		void onWrite(string msg);
		void onRead();
		void onClose();
		Socket& operator=(const Socket&);
		Socket(const Socket&){};
	private:
		char m_buffer[256];
		static Socket* m_instance;
		int m_sockfd;
		int m_newsockfd;
		int m_port;
		bool m_isOpened;
		bool m_isReading;
		bool m_isWriting;
		socklen_t m_cliLen;
		struct sockaddr_in m_servAddr;
		struct sockaddr_in m_cliAddr;
};

#endif
