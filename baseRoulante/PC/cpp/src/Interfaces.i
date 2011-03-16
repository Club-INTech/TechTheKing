%module libChessUp


%{
    #include "Interfaces.h"
    #include "Thread.h"
    #include "Singleton.h"
    #include "Server.h"
%}

%inline %{
    std::vector <Obstacle*> listeObstacles ;
%}


typedef enum {positif,negatif} SensDeplacement;
typedef enum{pince,independants} ModeBras;
typedef enum{bas,haut} ModeAimant;

std::string exec(char* cmd);
class InterfaceAsservissement;
std::vector<char> getTtyUSB();

class Thread{
    public:
        void ouvrirThread();
        void fermerThread();
        virtual ~Thread();
    protected:
        Thread();
        virtual void thread()=0;
    protected:
        boost::thread* m_thread;
        boost::mutex m_mutex;
};

#define TAILLE_BUFFER 256
    
class Socket : public Thread{
    public:
        static Socket* instance(int port);
        ~Socket();
    private:
        void thread();
        Obstacle* trouverObstacle();
        Socket(int port);
        void onOpen();
        void onWrite(string msg);
        void onRead();
        void onClose();
        Socket& operator=(const Socket&);
        Socket(const Socket&){};
    private:
        char m_buffer[TAILLE_BUFFER];
        static Socket* m_instance;
        int m_sockfd;
        int m_newsockfd;
        int m_port;
        bool m_isOpened;
        bool m_isReading;
        bool m_isWriting;
        socklen_t m_cliLen;
        struct sockaddr_in m_servAddr;
        struct sockaddr_in m_cliAddr;
};

class InterfaceAsservissement{
public:
    friend void detectionSerieUsb(InterfaceAsservissement* asserv); // ne devrait pas servir si on garde l'i2c
    void goTo(Point depart, Point arrivee,int nbPoints);
    void avancer(unsigned int distance, SensDeplacement sens);
    void tourner(unsigned int angle, SensDeplacement sens);
    InterfaceAsservissement(int precisionAStar);
private:
    void recupPosition();
private:
    AStar m_pathfinding;
    unsigned int vitesseMax;
    SerialStream m_liaisonSerie;
};

class InterfaceCapteurs : public Thread{
public:
    InterfaceCapteurs();
private:
    inline void traiterAbsenceObstacle();
    inline void traiterPresenceObstacle();
    void thread();
private:
    I2cBus* m_busI2c;
};

class InterfaceActionneurs {
public:
    InterfaceActionneurs();
    void hauteurBrasGauche(unsigned char pourcentageHauteur);
    void hauteurBrasDroit(unsigned char pourcentageHauteur);
    void angleBrasGauche(unsigned char pourcentageAngle);
    void angleBrasDroit(unsigned char pourcentageAngle);
    void positionAimantGauche(ModeAimant mode);
    void positionAimantDroit(ModeAimant mode);
    void setMode(ModeBras mode);
private:
    unsigned char pourcentageHauteurConversion(unsigned char pourcentage); // D'un pourcentage à une valeur entre 0 et 255
    unsigned int pourcentageAngleConversion(unsigned char pourcentage); // D'un pourcentage à une valeur entre 0 et 1023 à envoyer via i2c
    template <class T>  std::stack<unsigned char> decToBin(T dec);

private:
    bool m_modePince;
    I2cBus* m_busI2c;
};
