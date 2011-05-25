#include "Thread.h"

Thread::Thread(){
	
}

Thread::~Thread(){
	m_thread.join();
}

void Thread::ouvrirThread(){
	m_thread = boost::thread(&Thread::thread,this);
}

void Thread::fermerThread(){
	m_thread.interrupt();
}
