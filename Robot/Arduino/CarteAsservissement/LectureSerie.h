/**
 * \file LectureSerie.h
 * \brief header de LectureSerie.cpp
 */
 
#ifndef LectureSerie_h
#define LectureSerie_h

#include <HardwareSerial.h>

/**
 * \brief Se charge des fonctions d'asservissement
 */
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
