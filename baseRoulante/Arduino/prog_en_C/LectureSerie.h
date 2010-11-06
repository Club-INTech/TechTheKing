#ifndef LectureSerie_h
#define LectureSerie_h

#include "usart.h"

class LectureSerie {
	public:
		LectureSerie();
		
		void	traitement();
		
	private:
		bool	litEntier(int *);
		bool	litEntierLong(long int *);
};

extern LectureSerie lectureSerie;

#endif
