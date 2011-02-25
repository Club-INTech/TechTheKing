#include "Singleton.h"

template <class T> T Singleton<T>::m_instance=NULL;

template <class T> T& Singleton<T>::Instance(){
	if(m_instance==NULL){
		m_instance= new T();
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;


template <class T> T& Singleton<T>::template <class C>  Instance(){
	if(m_instance==NULL){
		m_instance= new T(C)
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}

template <class T> T& Singleton<T>::template <class C, class M>  Instance(){
	if(m_instance==NULL){
		m_instance= new T(C,M)
	}
	else{
		cerr<<"Instance déjà créée"<<endl;
	}
	return m_instance;
}
