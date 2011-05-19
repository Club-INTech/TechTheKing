#ifndef SERVER_H
#define SERVER_H

#include <iostream>
#include <cstdio>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <netinet/in.h>
#include "Obstacles.h"
#define TAILLE_BUFFER 256
    
class Socket{
    public:
        static Socket* Instance(int port);
        ~Socket();
        void getPions(const char* address);
        void onOpen();
    private:
        Obstacle* trouverObstacle(std::string trame);
        Socket(int port);
        void onWrite(std::string msg);
        std::string onRead();
        void onClose();
        Socket& operator=(const Socket&);
        Socket(const Socket&){};
    private:
        static Socket* m_instance;
        int m_sockfd;
        int m_port;
};

#endif
