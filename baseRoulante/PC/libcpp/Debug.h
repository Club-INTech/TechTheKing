#ifndef DEBUGGRAPHIQUE_H
#define DEBUGGRAPHIQUE_H

#include "Point.h"
#include "Obstacles.h"
#include "Consigne.h"

namespace Debug{
	void debugGraphique(vector<Point> listePoints);
	void debugConsignes(vector<Consigne> listeConsignes);
}

#endif