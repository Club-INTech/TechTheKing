#include "Socket.h"
#include <netdb.h> 
#include <sys/socket.h>

Socket* Socket::m_instance=NULL;

Socket::Socket(int port):m_port(port){
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

void Socket::getPions(const char* address){
	struct hostent *server;
	struct sockaddr_in serv_addr;
	int n;
	server = gethostbyname(address);
	std::cout<<address<<std::endl;
    if (server == NULL) {
        std::cerr << "ERROR, no such host\n" << std::endl;
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(m_port);
    if (connect(m_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        std::cerr << "Could not connect\n" << std::endl;
    
    onWrite("pions");
    onRead();
    
    close(m_sockfd);
}

void Socket::onOpen(){
	m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if (m_sockfd < 0)
        std::cerr << "ERROR opening socket" << std::endl;
}

std::string Socket::onRead(){
	char buffer[TAILLE_BUFFER];
	cout << "Reading from socket..." << endl;
	if (read(m_sockfd,buffer,255) < 0) 
         std::cerr << "ERROR reading from socket" << std::endl;
    cout << "Read : " << buffer << endl;
    return buffer;
}

void Socket::onWrite(std::string msg){
	msg.push_back('\n');
	char buffer[TAILLE_BUFFER];
	bzero(buffer,TAILLE_BUFFER);
	memcpy(buffer,msg.c_str(),msg.size());
	cout << "Writing to socket : " << buffer << endl;
	if (write(m_sockfd,buffer,strlen(buffer)) < 0) 
         std::cerr << "ERROR writing to socket" << std::endl;
}

void Socket::onClose(){
}

Obstacle* Socket::trouverObstacle(std::string trame){
    int i=1;
    std::string x,y;
    while(trame[i]!='y'){
        if(trame[i]<48 || trame[i]>57){
             return NULL;
        }
        else{
             x.push_back(trame[i]);
        }
        i++;
    }
    i++;
    while(trame[i]!='\n'){
        if(trame[i]<48 || trame[i]>57){
            cerr<<"Trame invalide"<<endl;
            return NULL;
        }
        else{
            y.push_back(trame[i]);
            i++;
        }
    }
    return (new CercleObstacle(atoi(x.c_str()),atoi(y.c_str())));
}
