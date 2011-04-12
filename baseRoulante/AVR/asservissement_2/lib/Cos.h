#include "TableCos.h"
#include <avr/pgmspace.h>
#include <stdint.h> 
#include "serial.h"

#define ABS(x) ( (x)>0?(x):-(x))

extern int getCos(float angleRadian);

extern float getSin(float angleRadian);
