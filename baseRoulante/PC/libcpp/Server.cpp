#include "Server.h"

Socket::Socket(int port):m_port(port){
}

void Socket::onOpen(){
	if( (m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0){
		std::cerr<<"Erreur lors de l'ouverture du Socket"<<std::endl;
	}
	bzero((char*)&m_servAddr,sizeof(m_servAddr));
	m_servAddr.sin_family=AF_INET;
	m_servAddr.sin_addr.s_addr=INADDR_ANY;
	m_servAddr.sin_port=htons(m_port);
	if(bind(m_sockfd,(struct sockaddr*)&m_servAddr,sizeof(m_servAddr))<0)
		std::cerr<<"Erreur lors du binding du socket"<<std::endl;
	listen(m_sockfd,5);
	m_cliLen = sizeof(m_cliAddr);
	if((m_newsockfd=accept(m_sockfd,(struct sockaddr*)&m_cliAddr,&m_cliLen))<0)
		std::cerr<<"Erreur lors de l'acceptation du socket"<<std::endl;
}

Obstacle* Socket::onRead(){
	bzero(m_buffer,sizeof(m_buffer));
	if(read(m_newsockfd,m_buffer,sizeof(m_buffer)-1)<0)
		std::cerr<<"Erreur lors de la lecture du socket"<<std::endl;
	unsigned int i=0;
	std::string x,y;
	while(m_buffer[i]!='\0'){
		switch(m_buffer[i]){
			case 'x':
			while(m_buffer[i]!='y'){
				if(m_buffer[i]=='\0'){
					cerr<<"Trame incomplète"<<endl;
					return NULL;
				}
				else if(m_buffer[i]<48 || m_buffer[i]>57){
					cerr<<"Trame invalide"<<endl;
					return NULL;
				}
				else{
					x.push_back(m_buffer[i]);
				}
			}
			case 'y':
			while(m_buffer[i]!='x'){
				if(m_buffer[i]=='\0'){
					cerr<<"Trame incomplète"<<endl;
					return NULL;
				}
				else if(m_buffer[i]<48 || m_buffer[i]>57){
					cerr<<"Trame invalide"<<endl;
					return NULL;
				}
				else{
					y.push_back(m_buffer[i]);
				}
			}
		}
		i++;
	}
	return(new cercleObstacle(atoi(x.c_str()),atoi(y.c_str())));
}

void Socket::onWrite(std::string msg){
	const char* msgTmp = msg.c_str();
	if(write(m_newsockfd,msgTmp,sizeof(msgTmp))<0)
		std::cerr<<"Erreur lors de l'écriture du message"<<std::endl;
}

void Socket::onClose(){
	close(m_sockfd);
	close(m_newsockfd);
}

Socket::~Socket(){
	
}
