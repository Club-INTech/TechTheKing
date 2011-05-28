#include "Socket.h"
#include <netdb.h> 
#include <sys/socket.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <Magick++.h>
Socket* Socket::m_instance=NULL;

Socket::Socket(int port){
}

Socket::~Socket(){
    
}


Socket* Socket::Instance(int port){
    if(m_instance==NULL){
        m_instance= new Socket(port);
    }
    else{
        cerr<<"Instance déjà créée. port : " << port << endl;
    }
    return m_instance;
}

void Socket::getPions(){
	boost::thread_group group;
	//group.create_thread(boost::bind(&Socket::getPions,this,"192.168.6.3"));
	group.create_thread(boost::bind(&Socket::getPions,this,"192.168.6.2"));
	//group.create_thread(boost::bind(&Socket::getPions,this,"192.168.6.4"));
	group.join_all();
}
void Socket::getPions(const char* address){
	int port = 42000;
	int sockfd = getFd(address);
	struct hostent *server;
	struct sockaddr_in serv_addr;
	int n;
	server = gethostbyname(address);
    if (server == NULL) {
        std::cerr << "ERROR, no such host\n" << std::endl;
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(port);
   
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        std::cerr << "Could not connect\n" << std::endl;
    }
    onWrite(sockfd,"pions");
    std::vector<Obstacle*> ranranru = trouverObstacles(onRead(sockfd));
    
    #ifdef DEBUG_GRAPHIQUE
    Magick::Image image( "img/table.png" );
	for(std::vector<Obstacle *>::iterator it=ranranru.begin();it!=ranranru.end();it++){
		(*it)->draw(&image);
	}
	image.display();
    #endif
    
    close(sockfd);
}


void Socket::onOpen(){
	int socketTmp;
	if((socketTmp=socket(AF_INET,SOCK_STREAM,0))<0){
		std::cerr << "Failed to create socket" << std::endl;
	}else{
		m_sockfds_map.insert(std::pair<const char* , int>("192.168.6.2",socketTmp));
	}
	if((socketTmp=socket(AF_INET,SOCK_STREAM,0))<0){
		std::cerr << "Failed to create socket" << std::endl;
	}else{
		m_sockfds_map.insert(std::pair<const char* ,int>("192.168.6.3",socketTmp));
	}
	if((socketTmp=socket(AF_INET,SOCK_STREAM,0))<0){
		std::cerr << "Failed to create socket" << std::endl;
	}else{
		m_sockfds_map.insert(std::pair<const char* ,int>("192.168.6.4",socketTmp));
	}
	
}

std::string Socket::onRead(int sockfd){
	char buffer[TAILLE_BUFFER];
	if (read(sockfd,buffer,255) < 0) 
         std::cerr << "ERROR reading from socket " << sockfd << std::endl;
    cout << "Read : " << buffer << endl;
    return std::string(buffer);
}

void Socket::onWrite(int sockfd,std::string msg){
	msg.push_back('\n');
	char buffer[TAILLE_BUFFER];
	bzero(buffer,TAILLE_BUFFER);
	memcpy(buffer,msg.c_str(),msg.size());
	if (write(sockfd,buffer,strlen(buffer)) < 0){
         std::cerr << "ERROR writing to socket" <<  sockfd  << std::endl;
	 }
}

int Socket::getFd(const char* address){
	boost::mutex::scoped_lock scoped_lock(m_mutex);
	int result = m_sockfds_map[address];
	return result;
}
void Socket::onClose(){
}

std::vector<Obstacle*> Socket::trouverObstacles(std::string trame){
	std::vector<Obstacle*> result;
    std::string x,y;
    if(trame[0]!='d'){
		return result;
	}
	else{
		int i=1;
		char currentCoord = 'x';
		while(trame[i]!='f'){
			switch(trame[++i]){
				case 'f':
				case 'x':
					if(currentCoord=='y'){
						std::cout << "x = " << x << " y = " << y << std::endl;
						result.push_back(new CercleObstacle(atoi(x.c_str()),atoi(y.c_str())));
					}
					x="";
					y="";
					currentCoord='x';
					break;
				case 'y':
					currentCoord='y';
					break;
				default:
					switch(currentCoord){
						case 'x':
							x.push_back(trame[i]);
							break;
						case 'y':
							y.push_back(trame[i]);
							break;
					}
			}
		}
		return result;
	}
}
