#include "TableCos.h"
#include <avr/pgmspace.h>
#include <stdint.h> 
#define ABS(x) ( (x)>0?(x):-(x))

extern long getCos(float angleRadian);

extern float getSin(float angleRadian);
