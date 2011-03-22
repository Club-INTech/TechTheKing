#ifndef DEBUG_H
#define DEBUG_H

#include "config.h"
#include "Point.h"
#include "Obstacles.h"
#include "Consigne.h"
#ifdef DEBUG_GRAPHIQUE
#include <Magick++.h>
#endif

namespace Debug{
#ifdef DEBUG_GRAPHIQUE
	void debugGraphique(vector<Point> listePoints);
#endif
	void debugConsignes(vector<Consigne> listeConsignes);
}

#endif