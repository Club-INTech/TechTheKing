#include "Server.h"
#include "config.h"
#include <list>


Socket* Socket::m_instance=NULL;

Socket::Socket(int port):Thread(), m_port(port){
   bzero(m_buffer,sizeof(m_buffer));
}

Socket::~Socket(){
    
}

void Socket::thread(){
   if(!onOpen())
      return;
   #ifdef DEBUG
      cout<<"Socket actif" << endl;
   #endif
   while(m_buffer[0]!='e'){
        onRead();
        switch(m_buffer[0]){
            case 'e':
               #ifdef DEBUG
               cout<<"Fermeture du thread"<<endl;
               #endif
               break;
            case 'd':
               #ifdef DEBUG
               cout<<"Réception d'une liste d'obstacle"<<endl;
               #endif
               m_mutex.lock();
               std::list<Obstacle*> listeRecue = analyserListeObstacle();
               if(listeRecue.empty()){
                   #ifdef DEBUG
                  cout<<"Echec."<<endl;
                  #endif
                  break;
               }
               #ifdef DEBUG
               cout<<"Ajout des obstacles reçus à la liste : "<<endl;
               #endif
               for(std::list<Obstacle*>::iterator it=listeRecue.begin();it!=listeRecue.end();it++){
                  #ifdef DEBUG
                  cout<< "( x : " << (*it)->x << " y : " << (*it)->y << " ) ; " ;
                  #endif
                  listeObstacles.push_back(*it);
               }
               #ifdef DEBUG
               cout<< endl;
               #endif
               m_mutex.unlock();
               break;
        }
    }
    onClose();
}


Socket* Socket::Instance(int port){
    if(m_instance==NULL){
         #ifdef DEBUG
         cout<<"Création du socket"<<endl;
         #endif
         m_instance= new Socket(port);
    }
    else{
         #ifdef DEBUG
         cout<<"Socket déjà crée : récupération de l'instance existante."<<endl;
         #endif
    }
    return m_instance;
}

bool Socket::onOpen(){
    #ifdef DEBUG
    cout<<"Ouverture du Socket sur le port " << m_instance->m_port << endl;
    #endif
    if( (m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0){
        std::cerr<<"Erreur lors de l'ouverture du Socket"<<std::endl;
        return false;
    }
    bzero((char*)&m_servAddr,sizeof(m_servAddr));
    m_servAddr.sin_family=AF_INET;
    m_servAddr.sin_addr.s_addr=INADDR_ANY;
    m_servAddr.sin_port=htons(m_port);
    #ifdef DEBUG
    cout<<"Binding du Socket"<<endl;
    #endif
    if(bind(m_sockfd,(struct sockaddr*)&m_servAddr,sizeof(m_servAddr))<0){
      std::cerr<<"Erreur lors du binding du socket"<<std::endl;
      return false;
   }
   listen(m_sockfd,5);
   m_cliLen = sizeof(m_cliAddr);
   return true;
}

void Socket::onRead(){
   #ifdef DEBUG
    cout<<"Début de l'écoute"<<endl;
    #endif
    bzero(m_buffer,sizeof(m_buffer));
    if((m_newsockfd=accept(m_sockfd,(struct sockaddr*)&m_cliAddr,&m_cliLen))<0)
        std::cerr<<"Erreur lors de l'acceptation du socket"<<std::endl;
    if(read(m_newsockfd,m_buffer,sizeof(m_buffer)-1)<0)
        std::cerr<<"Erreur lors de la lecture du socket"<<std::endl;
    #ifdef DEBUG
    cout<<"Fin de l'écoute. Message lu : " << m_buffer << endl;
    #endif
}

void Socket::onWrite(std::string msg){
   #ifdef DEBUG
   cout<<"Début de l'écriture du message : " << msg << endl;
   #endif
   const char* msgTmp = msg.c_str();
   if(write(m_newsockfd,msgTmp,sizeof(msgTmp))<0)
      std::cerr<<"Erreur lors de l'écriture du message"<<std::endl;
   else{
      #ifdef DEBUG
      cout<<"Message écrit." << endl;
      #endif
   }
}

void Socket::onClose(){
    close(m_sockfd);
    close(m_newsockfd);
}

std::list<Obstacle*> Socket::analyserListeObstacle(){
    std::list<Obstacle*> res;
    int i=1;
    std::string x,y;
    while(m_buffer[i]!='e'){
      while(m_buffer[i]!='y'){
         if(m_buffer[i]<48 || m_buffer[i]>57){
               cerr<<"Trame invalide"<<endl;
               return res;
         }
         else{
               x.push_back(m_buffer[i]);
         }
         i++;
      }
      i++;
      while(m_buffer[i]!='x'){
         if(m_buffer[i]<48 || m_buffer[i]>57){
               cerr<<"Trame invalide"<<endl;
               return res;
         }
         else{
               y.push_back(m_buffer[i]);
               i++;
         }
      }
      res.push_back(new CercleObstacle(atoi(x.c_str()),atoi(y.c_str())));
    }
    return res;
}
