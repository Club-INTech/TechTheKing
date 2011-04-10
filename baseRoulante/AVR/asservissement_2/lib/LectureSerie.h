#ifndef LectureSerie_h
#define LectureSerie_h

#define _DEBUG_

#include "serial.h"
#include "Manager.h"
#include <stdint.h> 

class LectureSerie {
	public:
		LectureSerie();
		
		void	traitement();
		
	private:
		bool	litEntier(int16_t *);
		bool	litEntierLong(int32_t *);
};

extern LectureSerie lectureSerie;

#endif
