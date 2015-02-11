/* Texture functions for cs580 GzLib	*/
#include    "stdafx.h" 
#include	"stdio.h"
#include	"Gz.h"
#include   <math.h>

GzColor	*image=NULL;
int xs, ys;
int reset = 1;

/* Image texture function */
int tex_fun(float u, float v, GzColor color)
{
  unsigned char		pixel[3];
  unsigned char     dummy;
  char  		foo[8];
  int   		i, j;
  FILE			*fd;
   float newu,newv;

  if (reset) {          /* open and load texture file */
    fd = fopen ("texture", "rb");
    if (fd == NULL) {
      fprintf (stderr, "texture file not found\n");
      exit(-1);
    }
    fscanf (fd, "%s %d %d %c", foo, &xs, &ys, &dummy);
    image = (GzColor*)malloc(sizeof(GzColor)*(xs+1)*(ys+1));
    if (image == NULL) {
      fprintf (stderr, "malloc for texture image failed\n");
      exit(-1);
    }

    for (i = 0; i < xs*ys; i++) {	/* create array of GzColor values */
      fread(pixel, sizeof(pixel), 1, fd);
      image[i][RED] = (float)((int)pixel[RED]) * (1.0 / 255.0);
      image[i][GREEN] = (float)((int)pixel[GREEN]) * (1.0 / 255.0);
      image[i][BLUE] = (float)((int)pixel[BLUE]) * (1.0 / 255.0);
      }

    reset = 0;          /* init is done */
	fclose(fd);
  }

/* bounds-test u,v to make sure nothing will overflow image array bounds */
/* determine texture cell corner values and perform bilinear interpolation */
/* set color to interpolated GzColor value and return */
 float ux,vy;
 ux=u;
 vy=v;
 if(ux<0)u=0;
 if(ux>1)u=1;
 if(vy<0)v=0;
 if(vy>1)v=1;
    newu=(ux)*(xs-1);
	newv=(vy)*(ys-1);
	ux=newu-floor(newu);
	vy=newv-floor(newv);
	int A[2],B[2],C[2],D[2];
	A[0]=floor(newu);
	A[1]=floor(newv);
	B[0]=ceil(newu);
	B[1]=floor(newv);
	C[0]=ceil(newu);
	C[1]=ceil(newv);
	D[0]=floor(newu);
	D[1]=ceil(newv);


	if(A[0]>xs-1)A[0]=0;
	if(B[0]>xs-1)B[0]=0;
	if(C[0]>xs-1)C[0]=0;
	if(D[0]>xs-1)D[0]=0;
	if(A[1]>ys-1)A[1]=0;
	if(B[1]>ys-1)B[1]=0;
	if(C[1]>ys-1)C[1]=0;
	if(D[1]>ys-1)D[1]=0;

	//if(A[0]>xs||B[0]>xs||C[0]>xs||D[0]>xs) ;else 
		//if(A[1]>ys||B[1]>ys||C[1]>ys||D[1]>ys);else
	//if((C[0]+C[1]*(ys-1))>(xs*ys));
		//	if((A[0]+A[1]*(ys-1))>(xs*ys));
			//		if((B[0]+B[1]*(ys-1))>(xs*ys));
			  //			if((D[0]+D[1]*(ys-1))>(xs*ys));
	for(i=0;i<3;i++)
    color[i]=ux*vy*image[C[0]+C[1]*xs][i]
			+(1-ux)*vy*image[D[0]+D[1]*xs][i]
			+ux*(1-vy)*image[B[0]+B[1]*xs][i]
			+(1-ux)*(1-vy)*image[A[0]+A[1]*xs][i];

	return GZ_SUCCESS;

}

/* Procedural texture function */
int ptex_fun(float u, float v, GzColor color)
{
	int j=rand()%10;
	int N=abs(j*100-1000);
	float ux,vy;
	float c1,c2;
	//c1=0.59;
	float Fr,Fi;
	c1=-0.4;
	c2=-0.6;
	ux=u;
	vy=v;
	int n=0;
	float lenght=0;

	while(n<N)
	{
	
		Fr=ux*ux+vy*vy+c1;
	Fi=2*ux*vy+c2;
	if((Fr+Fi)>256)
	{Fr=0;
	Fi=0;
	}
	n++;
	lenght=sqrt(Fr*Fr+Fi*Fi);
	ux=Fr;
	vy=Fi;
	}
	if(lenght<0) lenght=0;
	if(lenght>2)lenght=2;
	int i;
	float S,Si1,Si;
	float val[11]={0,0.2,0.4,0.6,0.8,1.0,1.2,1.4,1.6,1.8,2};
	GzColor LUT[11]={{0,0,0}
					,{0,0,0.5}
					,{0,0,1}
					,{0,0.5,0}
					,{0,0.5,0.5}
					,{0,1,0}
					,{0,1,1,}
					,{1,0,0}
					,{1,0,0.5}
					,{1,0,1}
					,{1,1,0}
	};
	/*LUT[0][RED]=LUT[0][GREEN]=LUT[0][BLUE]=0;
		LUT[0][RED]=LUT[0][GREEN]=0;LUT[0][BLUE]=0.5;
			LUT[0][RED]=LUT[0][GREEN]=0;LUT[0][BLUE]=1;
				LUT[0][RED]=LUT[0][BLUE]=0;LUT[0][GREEN]=0.5;
				  LUT[0][RED]=LUT[0][BLUE]=0;LUT[0][GREEN]=1;
				  LUT[0][RED]=0;LUT[0][BLUE]=LUT[0][GREEN]=0.5;
				  LUT[0][RED]=0;LUT[0][BLUE]=LUT[0][GREEN]=1;
				  LUT[0][RED]=1;LUT[0][BLUE]=LUT[0][GREEN]=0;
				   LUT[0][RED]=1;LUT[0][BLUE]=0;LUT[0][GREEN]=1;
				     LUT[0][RED]=1;LUT[0][BLUE]=0;LUT[0][GREEN]=1;

				    
    /* for(int j=0;j<11;j++)
	 {
		 LUT[j][RED]=LUT[j][RED]+0.00124654;
		 LUT[j][BLUE]=LUT[j][BLUE]+0.01224654;
		 LUT[j][GREEN]=LUT[j][GREEN]+0.03245654;
	 }*/
	 int g=0;
	for(i=0;i<11;i++)
	{
		if(val[i]-lenght>0)
		{
			S=lenght;
			Si1=val[i];
			if(i-1==0)Si=val[i];
			else
			Si=val[i-1];
			g=i;
			i=11;
			break;
		}
	}
	float a=(Si1-S)/(Si1-Si);
	float b=(S-Si)/(Si1-Si);

	/*color[RED]=abs(a*LUT[g-1][RED]+b*LUT[g][RED]-0.5);
	color[GREEN]=a*LUT[g-1][GREEN]+b*LUT[g][GREEN];
	color[BLUE]=abs(a*LUT[g-1][BLUE]+b*LUT[g][BLUE]-0.5);
	*/
	color[RED]=LUT[g][RED];
	color[GREEN]=LUT[g][GREEN];
	color[BLUE]=LUT[g][BLUE];


 return 1;
}

/* Free texture memory */
int GzFreeTexture()
{
	if(image!=NULL)
		free(image);
	return GZ_SUCCESS;
}
