#include "Interfaces.h"

Interface::Interface() : m_idCarte(Indefini){
	 m_liaisonSerie.SetBaudRate(SerialStreamBuf::BAUD_57600);
	 m_liaisonSerie.SetCharSize( SerialStreamBuf::CHAR_SIZE_8);
	 m_liaisonSerie.SetNumOfStopBits(1);
 }
 
std::string Interface::detectionSerieUsb(){
	 char charTmp;
	 SerialStream streamTmp;
	 streamTmp.SetBaudRate(SerialStreamBuf::BAUD_57600);
	 streamTmp.SetCharSize( SerialStreamBuf::CHAR_SIZE_8);
	 streamTmp.SetNumOfStopBits(1);
	 for(unsigned int i=48; i<58;i++){ // de 0 à 9 en ASCII
		 streamTmp.Open("/dev/ttyUSB"+i);
		 streamTmp<<"?"<<endl;
		 streamTmp>>charTmp;
		 if(charTmp==m_idCarte){
			streamTmp.Close();
			return "/dev/ttyUSB"+i;
		 }
	 }
	 cerr<<"Carte non détectée"<<endl;
	 return "";
}

InterfaceAsservissement::InterfaceAsservissement() : Interface(), Singleton<InterfaceAsservissement>(){
	m_idCarte=Asservissement;
	m_liaisonSerie.Open(detectionSerieUsb());
}