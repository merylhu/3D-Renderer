/*   CS580 HW   */
#include    "stdafx.h"  
#include	"Gz.h"
#include	"disp.h"
#include <iostream>
#include <fstream>
#include<string.h>

int GzNewFrameBuffer(char** framebuffer, int width, int height)
{
	GzPixel p;
	//int x= sizeof(p.blue)+sizeof(p.alpha)+sizeof(p.green)+sizeof(p.red)+sizeof(p.z);
	*framebuffer =new char[(sizeof(p.blue)+sizeof(p.red)+sizeof(p.green)) * width * height];
	// malloc(x*width*height*sizeof(char));//
	
	/* create a framebuffer:
 -- allocate memory for framebuffer : 
 -- pass back pointer 
 -- NOTE: this function is optional and not part of the API, but you may want to use it within the display function.
*/
	return GZ_SUCCESS;
}

int GzNewDisplay(GzDisplay	**display, int xRes, int yRes)
{
	 int i,j=0;
	 j=0;
	// xRes = 2;
	 //yRes=2;
	 //GzDisplay **d;
	if(xRes <= MAXXRES && yRes <= MAXYRES)
	{ 
			
		   (*display) = new GzDisplay;
		   (*display)->xres=(short)xRes;
		   (*display)->yres=(short)yRes;
		   //[xRes*yRes];
		   //for(int i =0;i<xRes*yRes;i++)
				(*display)->fbuf=new GzPixel [xRes*yRes];
				
		return GZ_SUCCESS;
	}
else return GZ_FAILURE;
}

			//for(j=0;j<yRes*xRes;j++)
			//{ 
				//(*display+j)->xres= (short)xRes;
				//(*display+j)->yres=(short)yRes;
				//(*display+j)->fbuf=new GzPixel;//new GzPixel;
	//(*display+j)->fbuf->red=255;
	//(*display+j)->fbuf->blue=255;
	//(*display+j)->fbuf->green=255;
	//(*display+j)->fbuf->alpha=1;
	//(*display+j)->fbuf->z=0;
	
	//**display=**d;
	//display[0,0]->xres=xRes;
	//display[0,0]->yres=yRes;
	 //d.yres= yRes;
	 //*display=d;
/* create a display:
  -- allocate memory for indicated resolution
  -- pass back pointer to GzDisplay object in display
*/
		

int GzFreeDisplay(GzDisplay	*display)
{
	free(display);
/* clean up, free memory */
	return GZ_SUCCESS;
}


int GzGetDisplayParams(GzDisplay *display, int *xRes, int *yRes)
{
   *xRes=display->xres;
   *yRes=display->yres;
/* pass back values for a display */
	return GZ_SUCCESS;
}


int GzInitDisplay(GzDisplay	*display)
{
   int xres;
   int yres;
   int s=GzGetDisplayParams(display,&xres,&yres);
   //GzDisplay dis;
   int i,j;
   display->fbuf=new GzPixel [xres*yres];
  // for(i=0;i<display->xres;i++)
  // {
   for(j=0;j<display->yres*display->xres;j++)
   {  
		   display->fbuf[j].red=4050;
		   display->fbuf[j].blue=4050;
		  display->fbuf[j].green=3500;
		   display->fbuf[j].alpha=1;
		   display->fbuf[j].z=MAXINT;
	   }
//}
	

return GZ_SUCCESS;

}
int GzPutDisplay(GzDisplay *display, int i, int j, GzIntensity r, GzIntensity g, GzIntensity b, GzIntensity a, GzDepth z)
{

	if(r>=4096)r=4095;
	if(g>=4096)g=4095;
	if(b>=4096)b=4095;
	if(r<0)r=0;
	if(g<0)g=0;
	if(b<0)b=0;
	int xres,yres;
	GzGetDisplayParams(display,&xres,&yres);
	if(i>=xres)i=xres-1;
	if(j>=yres)j=yres-1;
	if(i<0)i=0;
	if(j<0)j=0;
    display->fbuf[j+i*(display->xres-1)].blue=b;//d2b(b);
	display->fbuf[j+i*(display->xres-1)].red=r;//d2b(r);
	display->fbuf[j+i*(display->xres-1)].green=g;//d2b(g);
	display->fbuf[j+i*(display->xres-1)].alpha=a;//d2b(a);
	display->fbuf[j+i*(display->xres-1)].z=z;//d2b(z);
/* write pixel values into the display */
	return GZ_SUCCESS;
}


int GzGetDisplay(GzDisplay *display, int i, int j, GzIntensity *r, GzIntensity *g, GzIntensity *b, GzIntensity *a, GzDepth *z)
{
	*r=display->fbuf[j+i*(display->xres-1)].red;
	*g=display->fbuf[j+i*(display->xres-1)].green;
    *b=display->fbuf[j+i*(display->xres-1)].blue;
	*a=display->fbuf[j+i*(display->xres-1)].alpha;
	*z=display->fbuf[j+i*(display->xres-1)].z;

	/* pass back pixel value in the display */
		return GZ_SUCCESS;
}


int GzFlushDisplay2File(FILE* outfile, GzDisplay *display)
{

	int j=0;
	fprintf(outfile,"P6 %d %d 255\r",display->xres,display->yres);
	for(int j=0;j<display->yres;j++)
	{
	for(int i=0;i<display->xres;i++)
		{   
		    char c1=char (display->fbuf[j+i*(display->xres-1)].blue>>4);
			char c2=char (display->fbuf[j+i*(display->xres-1)].green>>4);
			char c3=char(display->fbuf[j+i*(display->xres-1)].red>>4);
			fprintf(outfile,"%c",c3);
			fprintf(outfile,"%c",c2);
			fprintf(outfile,"%c",c1);
		    
		}}
	//fclose(outfile);

/* write pixels to ppm file -- "P6 %d %d 255\r" */

	return GZ_SUCCESS;
}

int GzFlushDisplay2FrameBuffer(char* framebuffer, GzDisplay *display)
{ 
	short a,r,g,b;
	GzDepth z;
	
	int k=0;
	//GzInitDisplay(display);
	//framebuffer=
	for(int j=0;j<display->yres;j++)
	{
		for(int i=0;i<display->xres;i++)
		{
			GzGetDisplay(display,i,j,&r,&g,&b,&a,&z);
			//char c[3];
			char c1=char (b>>4);//display->fbuf[j+i*(display->xres-1)].blue>>4);
			char c2=char (g>>4);//display->fbuf[j+i*(display->xres-1)].green>>4);
			char c3=char(r>>4);//display->fbuf[j+i*(display->xres-1)].red>>4);
			//
			framebuffer[k++]=c1;
			framebuffer[k++]=c2;
			framebuffer[k++]=c3;
			

		//fprintf(outfile,"%c %d %d %c","P6",display[i,j].xres,display[i,j].yres,"255\n");
			//fprintf(outfile,"%c",char(display->fbuf->blue>>4));
			//fprintf(outfile,"%c",char(display->fbuf->green>>4));
			//fprintf(outfile,"%c",char(display->fbuf->red>>4));
		}
	}
	/* write pixels to framebuffer: 
		- Put the pixels into the frame buffer
		- Caution: store the pixel to the frame buffer as the order of blue, green, and red 
		- Not red, green, and blue !!!
	*/

	return GZ_SUCCESS;
}