#include "Singleton.h"
#include "Interfaces.h"

using namespace std;

Singleton* Singleton::m_instance=NULL;

Singleton* Singleton::Instance(){
	if(m_instance==NULL){
		m_instance= new Singleton();
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}

template<typename C> Singleton* Singleton::Instance(C a){
	if(m_instance==NULL){
		m_instance= new Singleton(a);
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}

template<typename C, typename M> Singleton* Singleton::Instance(C a,M b){
	if(m_instance==NULL){
		m_instance= new Singleton(a,b);
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}
