#include "TableCos.h"
#include <avr/pgmspace.h>
#define ABS(x) ( (x)>0?(x):-(x))
const float COS_TABLE[NOMBRE_COS] PROGMEM = { COS_LIST };


float getCos(float angleRadian){
  uint32_t t;
  float result;
  int offsetSigne=1;
  angleRadian=ABS(angleRadian);
  int quotient = (int) (angleRadian/(2*PI));
  angleRadian=angleRadian - (float) (quotient * 2 * PI) ;
  if(angleRadian>PI){
    angleRadian-=PI;
    offsetSigne=-1;
  }
  t = pgm_read_dword(&COS_TABLE[(int)(NOMBRE_COS*angleRadian/PI+0.5)]);
  result = offsetSigne*(*((float*)&t));
  return result; 
}

float getSin(float angleRadian){
  return getCos(angleRadian-PI/2);
}


int main(){
	int x=0;
	int y=0;
	while(1){
	}
}
