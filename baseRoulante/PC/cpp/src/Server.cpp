#include "Server.h"
#include "config.h"
#include <time.h>
#include <list>


Socket* Socket::m_instance=NULL;

Socket::Socket(int port):Thread(), m_port(port),m_isOpened(false),m_isAccepted(false),m_isRequesting(false){
   bzero(m_buffer,sizeof(m_buffer));
}

Socket::~Socket(){
    if(m_instance)
      delete(m_instance);
    onClose();
}

void Socket::request(){
    
    cout<<m_isRequesting<<endl;
    m_mutex.lock();
    m_isRequesting = true;
    m_mutex.unlock();
    cout<<m_isRequesting<<endl;
}
void Socket::thread(){
    
   
     while(1){

        if((m_newsockfd=accept(m_sockfd,(struct sockaddr*)&m_cliAddr,&m_cliLen))<0){
            std::cerr<<"Erreur lors de l'acceptation du socket"<<std::endl;
            m_isOpened=false;
            return;
        }
        else{
            #ifdef DEBUG
             cout<<"Socket acceptée"<<endl;
             #endif
        }   
            cout<<"ccccccccccccccccccccccccc"<<endl;
        while(!m_isRequesting){
            usleep(100000);
        }
            onWrite('p');
            bzero(m_buffer,sizeof(m_buffer));
            onRead();
            traitementReception();
            m_mutex.lock();
            m_isRequesting=false;
            m_mutex.unlock();
    }
      
}

void Socket::traitementReception(){
    switch(m_buffer[0]){
          case 't':
             onWrite('o');
             break;
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
                cout<< "( x : " << (*it)->getX() << " y : " << (*it)-> getY() << " ) ; " ;
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

void Socket::onOpen(){
   if(m_isOpened)
      return;
    #ifdef DEBUG
    cout<<"Ouverture du Socket sur le port " << m_instance->m_port << endl;
    #endif
    if( (m_sockfd = socket(AF_INET, SOCK_STREAM, 0)) <0){
        std::cerr<<"Erreur lors de l'ouverture du Socket"<<std::endl;
        m_isOpened=false;
    }
    bzero((char*)&m_servAddr,sizeof(m_servAddr));
    m_servAddr.sin_family=AF_INET;
    m_servAddr.sin_addr.s_addr=INADDR_ANY;
    m_servAddr.sin_port=htons(m_port);
    if(bind(m_sockfd,(struct sockaddr*)&m_servAddr,sizeof(m_servAddr))<0){
      std::cerr<<"Erreur lors du binding du socket"<<std::endl;
      m_isOpened=false;
      return;
   }
   #ifdef DEBUG
   cout<<"Socket bindé"<<endl;
   #endif
   listen(m_sockfd,5);
   m_cliLen = sizeof(m_cliAddr);
   m_isOpened=true;
   
}

void Socket::onRead(){
   #ifdef DEBUG
    cout<<"Début de l'écoute ; "<< m_buffer <<endl;
    #endif
    if(read(m_newsockfd,m_buffer,sizeof(m_buffer)-1)<0){
      std::cerr<<"Erreur lors de la lecture du socket"<<std::endl;
      return;
    }
    #ifdef DEBUG
    cout<<"Fin de l'écoute. Message lu :" << m_buffer << endl;
    #endif
}

void Socket::onWrite(std::string msg){
   const char* msgTmp = msg.c_str();
   #ifdef DEBUG
   cout<<"Début de l'écriture du message :" << msgTmp << endl;
   #endif
   if(write(m_newsockfd,msgTmp,sizeof(msgTmp))<0){
      std::cerr<<"Erreur lors de l'écriture du message"<<std::endl;
      return;
   }
   #ifdef DEBUG
   cout<<"Message écrit." << endl;
   #endif
   
}

void Socket::onWrite(char msg){
   #ifdef DEBUG
   cout<<"Début de l'écriture du message :" << msg << endl;
   #endif
   if(write(m_newsockfd,&msg,sizeof(msg))<0){
      std::cerr<<"Erreur lors de l'écriture du message"<<std::endl;
      return;
   }
   #ifdef DEBUG
   cout<<"Message écrit." << endl;
   #endif
}

void Socket::onClose(){
    if(m_isOpened){
        close(m_sockfd);
        m_isOpened=false;
    }
}

std::list<Obstacle*> Socket::analyserListeObstacle(){
   std::list<Obstacle*> res;
   std::string x,y;
   int i=1;
    char courant = m_buffer[i];
    char coordCourante='x';
    while(courant!='f'){
      courant=m_buffer[++i];
      if(courant=='x' || courant=='f'){
         Obstacle* obstacleCourant = new CercleObstacle(atoi(x.c_str()),atoi(y.c_str()));
         res.push_back(obstacleCourant);
         coordCourante='x';
      }
      else if(courant=='y'){
         coordCourante='y';
      }
      else if(courant<48 || courant>57){
         cerr<<"Trame (liste des obstacles) invalide"<<endl;
         return res;
      }
      else{
         switch(coordCourante){
            case 'x':
               cout<<x<<endl;
               x.push_back(courant);
               break;
            case 'y':
               y.push_back(courant);
               break;
         }
         cout<<coordCourante<<endl;
      }
   }
    return res;
}
