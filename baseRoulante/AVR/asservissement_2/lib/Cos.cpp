#include "Cos.h"

#include <avr/pgmspace.h>
#include <math.h>

#define ABS(x) (x > 0 ? x : -x)
#define FP_PI_OVER_TWO 102944
#define FP_TWO_PI 411775

#define TABLE_LENGTH 64
#define TABLE_STEP 1621//.160757

static inline int32_t trig_lookup(int32_t theta, int32_t* table);

static int32_t linspace[TABLE_LENGTH] PROGMEM = { 
	0, 1621, 3242, 4863, 6485, 8106, 9727, 11348, 
	12969, 14590, 16212, 17833, 19454, 21075, 22696, 24317, 
	25939, 27560, 29181, 30802, 32423, 34044, 35666, 37287, 
	38908, 40529, 42150, 43771, 45393, 47014, 48635, 50256, 
	51877, 53498, 55119, 56741, 58362, 59983, 61604, 63225, 
	64846, 66468, 68089, 69710, 71331, 72952, 74573, 76195, 
	77816, 79437, 81058, 82679, 84300, 85922, 87543, 89164, 
	90785, 92406, 94027, 95648, 97270, 98891, 100512, 102133
};

static int32_t fp_cos_table[TABLE_LENGTH] PROGMEM = {
	65536, 65516, 65456, 65356, 65215, 65035, 64815, 64556, 
	64257, 63919, 63541, 63125, 62670, 62176, 61645, 61076, 
	60470, 59826, 59146, 58430, 57678, 56890, 56068, 55212, 
	54322, 53398, 52442, 51454, 50434, 49384, 48303, 47193, 
	46053, 44886, 43691, 42470, 41222, 39949, 38652, 37331, 
	35988, 34622, 33235, 31828, 30401, 28956, 27492, 26013, 
	24517, 23006, 21481, 19943, 18393, 16831, 15260, 13679, 
	12089, 10492, 8889, 7280, 5667, 4050, 2431, 811
};

//FIXME Lazy implementations of tangent and sine.
int32_t fp_tan(int32_t theta) { 
	return fp_div(fp_sin(theta), fp_cos(theta));
}

int32_t fp_sin(int32_t theta) { 
	return fp_cos(theta + FP_PI_OVER_TWO);
}

//Quadratic interpolation.
int32_t fp_cos(int32_t theta) { 
	uint8_t n;	
	int8_t negative;
	int32_t x_n, x_np1, x_np2;
	int32_t y_n, y_np1, y_np2;
	int32_t dd_n, dd_np1, second_dd, result;
	
	//Move theta into [0, pi/2] w/ appropriate sign.
	theta = ABS(theta) % FP_TWO_PI; 

	if(theta > FP_PI) 
		theta = FP_TWO_PI - theta;
	
	if(theta > FP_PI_OVER_TWO) {
		theta = FP_PI - theta;
		negative = 1;
	}

	//Find the nearest table values. FIXME
	n = theta / TABLE_STEP;
	while( n < TABLE_LENGTH - 1 && (x_np1 = pgm_read_dword(&linspace[n+1]))) 
		n++;
	
	//theta is between x_{n} and x_{n+1}
	
	if(n == TABLE_LENGTH - 1) { 
		//Perform linear interpolation, since we're close to zero anyway.
		x_n = pgm_read_dword(&linspace[TABLE_LENGTH - 1]);
		y_n = pgm_read_dword(&fp_cos_table[TABLE_LENGTH - 1]);

		result = fp_mult(fp_div(FP_PI_OVER_TWO - x_n, 0 - y_n), theta - x_n) + y_n;
		return negative ? -result : result;
	}

	if(n == TABLE_LENGTH) { 
		
		return 0;
	}
	
	//Address the general case. Quadratic interpolation.
	//Load in the necessary values.
	x_n = pgm_read_dword(&linspace[n]);
	x_np2  = pgm_read_dword(&linspace[n + 2]);
	
	y_n = pgm_read_dword(&fp_cos_table[n]);
	y_np1 = pgm_read_dword(&fp_cos_table[n + 1]);
	y_np2 = pgm_read_dword(&fp_cos_table[n + 2]);

	//Calculate first divided differences.
	dd_n = fp_div(y_np1 - y_n, x_np1 - x_n);
	dd_np1 = fp_div(y_np2 - y_np1, x_np2 - x_np1);

	//Calculate the second divided difference.
	second_dd = fp_div(dd_np1 - dd_n, x_np2 - x_n);
	
	result = fp_mult(fp_mult(second_dd, theta - x_n), theta - x_np1)  
		   	+ fp_mult(dd_n, theta - x_n) + y_n;

	return negative ? -result : result;
}

//FIXME I didn't write these functions very carefully...
int32_t fp_mult(int32_t a, int32_t b) { 
	return (int32_t) (((int64_t)a) * ((int64_t)b)) >>  16;
}

int32_t fp_div(int32_t a, int32_t b) { 
	return (int32_t) ((int64_t)a << 16) / (int64_t)b;
}



