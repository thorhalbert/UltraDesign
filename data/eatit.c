#include <stdio.h>
#include <math.h>

#define max(a,b) (((a)>(b))?(a):(b))

float seis[3][1800];

float mx[3],mn[3],df[3];

void gfile(num,filen)
int num;
char *filen;
{

	FILE *gf;
	int i;
	float iv;
	
	gf = fopen(filen,"r");
	if (gf==NULL) {
		printf("Cannot open %s\n",filen);
		exit(100);
	}
	
	mx[num] = -1e20;
	mn[num] = 1e20;
	
	for (i=0; i<1800; i++) {
		fscanf(gf,"%f",&iv);
		if (iv>mx[num]) mx[num] = iv;
		if (iv<mn[num]) mn[num] = iv;
		seis[num][i]=iv;
		/* fprintf(stderr,"%d %d %g\n",num,i,iv); */
	}
	
	fclose(gf);
	
	df[num] = mx[num] - mn[num];
	
	for (i=0; i<1800; i++) 
		seis[num][i] -= mn[num];

}

main()
{

	int i,j;
	float ux,uy,tv;

	gfile(0,"anmolhz.dat");
	gfile(1,"anmolhn.dat");
	gfile(2,"anmolhe.dat");

	tv = max(max(df[0],df[1]),df[2]);

	for (i=0; i<3; i++)
	  for (j=0; j<1800; j++) 
	  	seis[i][j] /= (tv / 3.0);

	for (i=0; i<3; i++) {
	  switch(i) {
	  case 0:
	  	printf("create layer lhz\n");
		printf("set current/layer lhz\n");
		break;
	  case 1:
	  	printf("create layer lhn\n");
		printf("set current/layer lhn\n");
		break;
	  case 2:
	  	printf("create layer lhe\n");
		printf("set current/layer lhe\n");
		break;
	  }
	
	  ux = 0.0;
	  uy = seis[i][0];
	  
	  printf("line %g,%g\n",ux,uy);
	
	  for (j=1; j<1800; j++) {
		ux += (1.0/1800.0) * 8.0;
		uy = seis[i][j];

		printf("line/cont %g,%g\n",ux,uy);
	  }
	}
}
