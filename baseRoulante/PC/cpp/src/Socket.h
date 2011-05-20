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
#include <map>
#include <boost/thread/mutex.hpp>
#define TAILLE_BUFFER 256
    
class Socket{
    public:
        static Socket* Instance(int port);
        ~Socket();
        void onOpen();
        void getPions();
    private:
		int getFd(const char* address);
		void getPions(const char* address);
        std::vector<Obstacle*> trouverObstacles(std::string trame);
        Socket(int port);
        void onWrite(int sockfd,std::string msg);
        std::string onRead(int sockfd);
        void onClose();
        Socket& operator=(const Socket&);
        Socket(const Socket&){};
    private:
        static Socket* m_instance;
        boost::mutex m_mutex;
        std::map<const char*,int> m_sockfds_map;
};

#endif
