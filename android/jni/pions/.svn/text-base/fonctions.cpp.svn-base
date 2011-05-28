#include "param.h"
#include "android.h"
#include "fonctions.h"

void print_tab(std::string txt, float t[])
{
	#if NBR_CLASSES == 3
	LOGI((txt + ": " + convertString(t[0]) + " " + convertString(t[1]) + " " + convertString(t[2])).c_str());
	#else
	LOGI((txt + ": " + convertString(t[0]) + " " + convertString(t[1]) + " " + convertString(t[2]) + " " + convertString(t[3])).c_str());
	#endif
}

void resetTab(float t[])
{
	int c;
	for (c=0; c<NBR_CLASSES; c++)
    {
		t[c] = 0;
	}
}

int max(float t[])
{
    int i;
    float max = 0;
    int imax = 0;

    for (i=0; i<NBR_CLASSES; i++)
    {
        if (t[i] > max)
        {
            imax = i;
            max = t[i];
        }
    }
    return imax;
}

void print_liste(std::list<Point> liste)
{
	if(!liste.empty())
	{
		for(std::list<Point>::iterator it=liste.begin() ; it != liste.end() ; ++it)
		{
			LOGI(("x=" + convertString(it->x) + " y=" + convertString(it->y)).c_str());
		}
	}
}
