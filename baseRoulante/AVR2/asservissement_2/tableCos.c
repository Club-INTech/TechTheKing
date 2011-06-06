#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int16_t main(int16_t argc, char* argv[]){
	FILE* f=fopen("./lib/TableCos.h","w+");
	int16_t i;
	if(argc==1){
		fprintf(stderr,"usage : %s N\n",argv[0]);
		exit(1);
	}
	int16_t N=atoi(argv[1]);
	if(!f){
		fprintf(stderr,"couldn't open TableCos.h");
	}
	fprintf(f,"#define PI %f\n",M_PI);
	fprintf(f,"#define NOMBRE_COS %d\n",N);
	fprintf(f,"#define COS_LIST ");
	for(i=0;i<(N-1);i++){
		fprintf(f,"%d,",(int) (cos(M_PI*i/(N-1))*10000) );
	}
	fprintf(f,"%f",cos(M_PI*i/(N)));
	fclose(f);
	return 0;
}
