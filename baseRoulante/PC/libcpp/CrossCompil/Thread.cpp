#include "Thread.h"

Thread::Thread():m_thread(NULL){
	
}

Thread::~Thread(){
	m_thread->join();
}

void Thread::ouvrirThread(){
	m_thread = new boost::thread(&Thread::thread,this);
}

void Thread::fermerThread(){
	m_thread->interrupt();
}
