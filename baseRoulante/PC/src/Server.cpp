#include "Server.h"

Socket::Socket(int port):Thread(), m_port(port){
}

Socket::~Socket(){
    
}

void Socket::thread(){
    onOpen();
    while(m_buffer[0]!='e'){
        onRead();
        switch(m_buffer[0]){
            case 'e':
            cout<<"Fermeture du thread"<<endl;
            break;
            case 'x':
            m_mutex.lock();
            listeObstacles.push_back(trouverObstacle());
            m_mutex.unlock();
            break;
            case 'y':
            cerr<<"La trame ne peut pas commencer par l'ordonnée"<<endl;
            break;
        }
    }
    onClose();
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
}

void Socket::onRead(){
    bzero(m_buffer,sizeof(m_buffer));
    if((m_newsockfd=accept(m_sockfd,(struct sockaddr*)&m_cliAddr,&m_cliLen))<0)
        std::cerr<<"Erreur lors de l'acceptation du socket"<<std::endl;
    cout<<"début lecture : " << m_buffer[0]<<endl;
    if(read(m_newsockfd,m_buffer,sizeof(m_buffer)-1)<0)
        std::cerr<<"Erreur lors de la lecture du socket"<<std::endl;
    cout<<"fin lecture : " << m_buffer[0]<<endl;
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

Obstacle* Socket::trouverObstacle(){
    int i=1;
    std::string x,y;
    while(m_buffer[i]!='y'){
        if(m_buffer[i]<48 || m_buffer[i]>57){
             return NULL;
        }
        else{
             x.push_back(m_buffer[i]);
        }
        i++;
    }
    i++;
    while(m_buffer[i]!='\n'){
        if(m_buffer[i]<48 || m_buffer[i]>57){
            cerr<<"Trame invalide"<<endl;
            return NULL;
        }
        else{
            y.push_back(m_buffer[i]);
            i++;
        }
    }
    return(new CercleObstacle(atoi(x.c_str()),atoi(y.c_str())));
}
