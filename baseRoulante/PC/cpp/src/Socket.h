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
		void getPions(const char* address, std::vector<Obstacle*>& Obstacles);
		void trouverObstacles(std::string trame, std::vector<Obstacle*>& Obstacles);
        Socket(int port);
        void onWrite(int sockfd,std::string msg);
        std::string onRead(int sockfd);
        void onClose();
        Socket& operator=(const Socket&);
        bool comparerPions(std::vector< std::pair<Obstacle*,int> >::iterator a, std::vector<Obstacle*>::iterator b);
		void printVector(std::vector<Obstacle*> v);
		std::vector< std::pair<Obstacle*,int> > fusionResultats(std::vector<Obstacle*> t1, std::vector<Obstacle*> t2, std::vector<Obstacle*> t3, int niveau);
		void ajouterPion(std::vector< std::pair<Obstacle*,int> > &v, std::vector<Obstacle*>::iterator p);
        Socket(const Socket&){};
    private:
        static Socket* m_instance;
        boost::mutex m_mutex;
        std::map<const char*,int> m_sockfds_map;
};

#endif
