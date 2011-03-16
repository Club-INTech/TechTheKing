#include "Singleton.h"

using namespace std;

template <class T> T Singleton<T>::m_instance=NULL;

template <class T> T& Singleton<T>::Instance(){
	if(m_instance==NULL){
		m_instance= new T();
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}

template <class T> template<typename C> T& Singleton<T>::Instance(C a){
	if(m_instance==NULL){
		m_instance= new T(a);
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}

template <class T> template<typename C, typename M> T& Singleton<T>::Instance(C a,M b){
	if(m_instance==NULL){
		m_instance= new T(a,b);
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}
