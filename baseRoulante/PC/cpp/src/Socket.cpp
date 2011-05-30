#include "Socket.h"
#include <netdb.h> 
#include <sys/socket.h>
#include <boost/thread.hpp>
#include <boost/bind.hpp>
#include <Magick++.h>

#define TOLERANCE_DISTANCE 10

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

void Socket::getAllPions(){
	#ifdef DEBUG
		std::cout << "Récupération des informations" << std::endl;
	#endif
    boost::thread_group group;
    std::vector<Obstacle*> listeObstacles1;
    std::vector<Obstacle*> listeObstacles2;
    std::vector<Obstacle*> listeObstacles3;
    group.create_thread(boost::bind(&Socket::getPions,this,"192.168.6.2",&listeObstacles1));
    group.create_thread(boost::bind(&Socket::getPions,this,"192.168.6.3",&listeObstacles2));
    group.create_thread(boost::bind(&Socket::getPions,this,"192.168.6.4",&listeObstacles3));
    group.join_all();
    
    std::vector< std::pair<Obstacle*,int> > fusion;
    fusionResultats(fusion, listeObstacles1,listeObstacles2,listeObstacles3,1);
    
    printVector(fusion);
    
	/*
    #ifdef DEBUG_GRAPHIQUE
    Magick::Image image( "img/table.png" );
    for(std::vector< std::pair<Obstacle*,int> >::iterator  it=fusion.begin();it!=fusion.end();it++){
        (it->first)->draw(&image);
    }
    image.display();
    #endif
    */
    
    
    
}
void Socket::getPions(const char* address,std::vector<Obstacle*>* Obstacles){
	#ifdef DEBUG
		std::cout << "Récupération de " << address << std::endl;
	#endif
    int port = 42000;
    int sockfd = getFd(address);
    struct hostent *server;
    struct sockaddr_in serv_addr;
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
   
    #ifdef DEBUG
		std::cout << "Connection à " << address << std::endl;
	#endif
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {
        std::cerr << "Could not connect\n" << std::endl;
    }
    #ifdef DEBUG
		std::cout << "Ecriture sur " << address << std::endl;
	#endif
    onWrite(sockfd,"pions");
    trouverObstacles(onRead(sockfd),Obstacles);
    
    /*
    #ifdef DEBUG_GRAPHIQUE
    Magick::Image image( "img/table.png" );
    for(std::vector<Obstacle*>::iterator it=Obstacles.begin();it!=Obstacles.end();it++){
        (*it)->draw(&image);
    }
    image.display();
    #endif
    */
    
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

void Socket::trouverObstacles(std::string trame, std::vector<Obstacle*>* Obstacles){
    std::string x,y;
    if(trame[0]!='d'){
        return ;
    }
    else{
        int i=1;
        char currentCoord = 'x';
        while(trame[i]!='f'){
            switch(trame[++i]){
                case 'f':
                case 'x':
                    if(currentCoord=='y'){
                        //std::cout << "x = " << x << " y = " << y << std::endl;
                        Obstacles->push_back(new CercleObstacle(atoi(x.c_str()),atoi(y.c_str())));
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
    }
}

// Intersection des résultats (au moins 2 téléphones nécessaires)
void Socket::fusionResultats(std::vector< std::pair<Obstacle*,int> >& resultatFusion, std::vector<Obstacle*>& t1, std::vector<Obstacle*>& t2, std::vector<Obstacle*>& t3, int niveau)
{
	std::vector<Obstacle*>::iterator it;
	std::vector< std::pair<Obstacle*,int> > resultatFusion2;
	// Une double boucle pour 3 instructions c'est mal
	for(it=t1.begin();it!=t1.end();it++)
	{
		ajouterPion(resultatFusion2,it);
	}
	
	for(it=t2.begin();it!=t2.end();it++)
	{
		ajouterPion(resultatFusion2,it);
	}
	
	for(it=t3.begin();it!=t3.end();it++)
	{
		ajouterPion(resultatFusion2,it);
	}
	
	// Tri des résultats en fonction du niveau
	for(std::vector< std::pair<Obstacle*,int> >::iterator it2=resultatFusion2.begin();it2!=resultatFusion2.end();it2++)
	{
		if (it2->second >= niveau){
			resultatFusion.push_back(*it2);
		}
	}
}

// True si les 2 pions sont les mêmes, false sinon 
bool Socket::comparerPions(std::vector< std::pair<Obstacle*,int> >::iterator& a, std::vector<Obstacle*>::iterator& b)
{
	double d = (a->first)->rayon(**b);
	return (d < TOLERANCE_DISTANCE);
}

// Vérifie si un pion est déjà présent, l'ajoute sinon
void Socket::ajouterPion(std::vector< std::pair<Obstacle*,int> > &v, std::vector<Obstacle*>::iterator& p)
{
	std::vector< std::pair<Obstacle*,int> >::iterator it;
	bool present = false;
	
	for(it=v.begin();it!=v.end();it++)
	{
		// Si pion déjà présent, on augmente le poids
		if (comparerPions(it,p))
		{
			present = true;
			
			// Calcul du barycentre
			(it->first)->setX( ( it->second * (it->first)->getX() + (*p)->getX() ) / (it->second + 1) );
			(it->first)->setY( ( it->second * (it->first)->getY() + (*p)->getY() ) / (it->second + 1) );
			
			// Incrémentation du poids
			it->second += 1;
		}
	}
	
	// Ajout du pion s'il n'est pas déjà présent
	if (!present)
	{
		// new CercleObstacle((*p)->getX(),(*p)->getY()) ?
		v.push_back(std::make_pair<Obstacle*,int>(new CercleObstacle((*p)->getX(),(*p)->getY()),1));
	}
}

void Socket::printVector(std::vector< std::pair<Obstacle*,int> >& v)
{	
	for(std::vector< std::pair<Obstacle*,int> >::iterator it=v.begin();it!=v.end();it++)
	{
		std::cout << ((it->first))->getX() << " " << ((it->first))->getY() << " n=" << it->second << std::endl;
	}
}
