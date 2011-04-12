#include "Cos.h"
#include "TableCos.h"
#include <avr/pgmspace.h>

PROGMEM prog_int16_t COS_TABLE[NOMBRE_COS] = {COS_LIST};

int getCos(float angleRadian){
  uint32_t t;
  int result;
  int16_t offsetSigne=1;
  angleRadian=ABS(angleRadian);
  int16_t quotient = (int) ( angleRadian / PI );
  angleRadian=angleRadian - (float) ( quotient * PI) ;
  if(angleRadian>PI){
    angleRadian-=PI;
    offsetSigne=-1;
  }
  result = pgm_read_dword(COS_TABLE + (int)(NOMBRE_COS*angleRadian/PI+0.5));
  return result;
}

float getSin(float angleRadian){
  return getCos(angleRadian-PI/2);
}
