/* CS580 Homework 3 */

#include	"stdafx.h"
#include	"stdio.h"
#include	"math.h"
#include	"Gz.h"
#include	"rend.h"
#define pi 3.14159265
#define V1 0
#define V2 1
#define V3 2
int TOS=0;
int XNormlevel=0;
#define AAKERNEL_SIZE 6

float       AAFilter[AAKERNEL_SIZE][3] 	=/* each sample is defined by Xshift, Yshift, weight*/
{  -0.52, 0.38, 0.128,                  0.41, 0.56, 0.119,                     0.27, 0.08, 0.294,
-0.17, -0.29, 0.249,                    0.58, -0.55, 0.104,                   -0.31, -0.71, 0.106    };
int GzCurrentValue(int z,GzRender *render)
{
	render->current=z;
	return GZ_SUCCESS;
}
int NormalizeXnorm(GzRender *render,GzMatrix *Matrix)
{
	float K=1/sqrt(pow(render->Xnorm[XNormlevel][0][0],2)+pow(render->Xnorm[XNormlevel][0][1],2)+pow(render->Xnorm[XNormlevel][0][2],2)
		+pow(render->Xnorm[XNormlevel][0][3],2));
	for(int i=0;i<4;i++)
		for(int j=0;j<4;j++)
			(*Matrix)[i][j]=render->Xnorm[XNormlevel][i][j]*K;
	return GZ_SUCCESS;

}
int round(float x)
{
	return floor(x+0.5);
}
short	ctoi(float color)		/* convert float color to GzIntensity short */
{
  return(short)((int)(color * ((1 << 12) - 1)));
}

int GzPutCamera(GzRender *render, GzCamera *camera)
{
	render->camera.FOV=camera->FOV;
	for(int ab=0;ab<3;ab++)
		{
			render->camera.lookat[ab]=camera->lookat[ab];
			render->camera.position[ab]=camera->position[ab];
			render->camera.worldup[ab]=camera->worldup[ab];
	    }
/*
- overwrite renderer camera structure with new camera definition
*/
	return GZ_SUCCESS;	
}

int GzRotXMat(float degree, GzMatrix mat)
{
	int i,j;
	//for(i=0;i<4;i++)
		//for(j=0;j<4;j++)
			//mat[i][j]=0;
	mat[0][0]=1;
	mat[1][1]=mat[2][2]=cos(degree*pi/180.0);
	mat[2][1]=sin(degree*pi/180.0);
	mat[1][2]=-1*mat[2][1];
// Create rotate matrix : rotate along x axis
// Pass back the matrix using mat value

	return GZ_SUCCESS;
}


int GzRotYMat(float degree, GzMatrix mat)
{
	int i,j;
	//for(i=0;i<4;i++)
		//for(j=0;j<4;j++)
			//mat[i][j]=0;
	mat[1][1]=1;
	mat[0][0]=mat[2][2]=cos(degree*(pi/180.0));
	mat[0][2]=sin(degree*(pi/180.0));
	mat[2][0]=-1*mat[0][2];
// Create rotate matrix : rotate along y axis
// Pass back the matrix using mat value

	return GZ_SUCCESS;
}


int GzRotZMat(float degree, GzMatrix mat)
{
	int i,j;
	//for(i=0;i<4;i++)
		//for(j=0;j<4;j++)
			//mat[i][j]=0;
	mat[2][2]=1;
	mat[0][0]=mat[1][1]=cos(degree*pi/180.0);
	mat[1][0]=sin(degree*pi/180.0);
	mat[0][1]=-1*mat[1][0];
// Create rotate matrix : rotate along z axis
// Pass back the matrix using mat value

	return GZ_SUCCESS;
}


int GzTrxMat(GzCoord translate, GzMatrix mat)
{
	
	int i,j;
	//for(i=0;i<4;i++)
		//for(j=0;j<4;j++)
			//{
				//mat[i][j]=0;
				//mat[i][i]=1;
			//}
		mat[0][3]=translate[0];
		mat[1][3]=translate[1];
		mat[2][3]=translate[2];
// Create translation matrix
// Pass back the matrix using mat value

	return GZ_SUCCESS;
}


int GzScaleMat(GzCoord scale, GzMatrix mat)
{
	for(int i=0;i<3;i++)
	{
			mat[i][i]=scale[i];
	    }
	//mat[3][3]=1;

// Create scaling matrix
// Pass back the matrix using mat value

	return GZ_SUCCESS;
}


//----------------------------------------------------------
// Begin main functions

int GzNewRender(GzRender **render, GzDisplay	*display)
{
		int i,j;

	(*render)=new GzRender;
		GzInitDisplay(display);
			(*render)->display=display;
			for(i=0;i<AAKERNEL_SIZE;i++)
		{
   			GzNewDisplay(&(*render)->displayfinal[i], 256, 256);
		GzInitDisplay((*render)->displayfinal[i]);
			}
		for(i=0;i<display->xres*display->yres;i++)
				(*render)->display->fbuf[i].z=MAXINT;
		(*render)->matlevel=0;
		(*render)->numlights=0;
		//(*render)->open
		//intialize Ximage first thingy
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
		{
			(*render)->Ximage[0][i][j]=(*render)->Xnorm[0][i][j]=0;
			if(i==j)
			(*render)->Ximage[0][i][j]=(*render)->Xnorm[0][i][j]=1;//overwrites it to identity 
		}
		
	for( i=0;i<4;i++)
		for( j=0;j<4;j++)
		{
			(*render)->Xsp[i][j]=0;
		}
		(*render)->Xsp[0][0]=(*render)->Xsp[0][3]=(*render)->display->xres/2;
		(*render)->Xsp[1][1]=-1*(*render)->display->yres/2;
		(*render)->Xsp[1][3]=(*render)->display->yres/2;
		(*render)->Xsp[2][2]=MAXINT;
		(*render)->Xsp[3][3]=1;
        (*render)->Xsp; 
	 GzPushMatrix(*render,(*render)->Xsp);
 (*render)->camera.FOV=35.0;//DEFAULT_FOV;
 //(*render)->camera.Xiw=new GzMatrix;
 
 for(i=0;i<3;i++)
	{
		(*render)->camera.lookat[i]=0;
		(*render)->camera.worldup[i]=0;
 }
 (*render)->camera.worldup[1]=1;
 (*render)->camera.position[0]=-10;
  (*render)->camera.position[1]=5; 
  (*render)->camera.position[2]=-10;
 //camera.FOV=DEFAULT_FOV;
 return GZ_SUCCESS;
	return GZ_SUCCESS;

}


int GzFreeRender(GzRender *render)
{
	free(render);
	return GZ_SUCCESS;
}

int GzPushXNorm(GzRender *render, GzMatrix	matrix)
{ int i,j,k;
  float c;
if(XNormlevel>MATLEVELS)
	  return GZ_FAILURE;
float Kval = 1/sqrt((matrix[0][0]*matrix[0][0] + matrix[0][1]*matrix[0][1] +
					matrix[0][2]*matrix[0][2] + matrix[0][3]*matrix[0][3]));
for(i=0;i<4;i++)
	for(j=0;j<4;j++)
		matrix[i][j]=matrix[i][j]*Kval;
matrix[0][0];
matrix[1][0];
matrix[2][0];
matrix[3][0];

XNormlevel++;
	for(i=0;i<4;i++)
		  for(j=0;j<4;j++)
		  { c=0;
			  if( XNormlevel==0)
				  for(k=0;k<4;k++)
					c+=matrix[k][j];
			  if( XNormlevel>0)
				  for(k=0;k<4;k++)
					c+=render->Xnorm[ XNormlevel-1][i][k]*matrix[k][j];
			   render->Xnorm[ XNormlevel][i][j]=c;
	}
	return GZ_SUCCESS;
}
int GzBeginRender(GzRender *render)
{
	  int i,j;
	  render->camera;
	 
	//Building Xpi
	float dinverse;
	if((render->camera.FOV/2.0)*(pi/180.0)==90.0);else
		dinverse=tan((render->camera.FOV/2.0)*(pi/180.0));
		//tan(render->camera.FOV/2.0);//
	
	//(*render)->display->fbuf
	for(i=0;i<4;i++)
		for(j=0;j<4;j++)
			if(i==j)
				render->camera.Xpi[i][j]=1;
			else
				render->camera.Xpi[i][j]=0;
	render->camera.Xpi[2][2]=dinverse;
	render->camera.Xpi[3][2]=dinverse;
	GzPushMatrix(render,render->camera.Xpi);

	//Building Xiw
	GzCoord xvect,yvect,zvect,upstar;
	float detcl=0.0;
	//Zvector
	for(i=0;i<3;i++)
		detcl+=pow(render->camera.lookat[i]-render->camera.position[i],2);
	detcl=sqrt(detcl);
	for(i=0;i<3;i++)
		zvect[i]=(render->camera.lookat[i]-render->camera.position[i])/detcl;
	zvect;
	//Up'vector
	float udotz=0.0;
	for(i=0;i<3;i++)
		udotz+=render->camera.worldup[i]*zvect[i];
    for(i=0;i<3;i++)
		upstar[i]=render->camera.worldup[i]-(udotz)*zvect[i];
	//Yvector;
	 float detupstar=0.0;
	 for(i=0;i<3;i++)
		 detupstar+=pow(upstar[i],2);
	     detupstar=sqrt(detupstar);
	 for(i=0;i<3;i++)
		 yvect[i]=upstar[i]/detupstar;
	 //XVector
	 xvect[0]=yvect[1]*zvect[2]-yvect[2]*zvect[1];//YyZz-YzZy
	 xvect[1]=yvect[2]*zvect[0]-yvect[0]*zvect[2];//YzZx-YxZz
	 xvect[2]=yvect[0]*zvect[1]-yvect[1]*zvect[0];//YxZy-YyZx
	 //Make Xiw
	 float cx,cy,cz;
	 cx=cy=cz=0;
	 for(i=0;i<3;i++)
	 {
			 render->camera.Xiw[0][i]=xvect[i];
			 render->camera.Xiw[1][i]=yvect[i];
			 render->camera.Xiw[2][i]=zvect[i];
	 }
	 for(i=0;i<3;i++)
	 {
		 cx+=-1*xvect[i]*render->camera.position[i];
		  cy+=-1*yvect[i]*render->camera.position[i];
		   cz+=-1*zvect[i]*render->camera.position[i];
	 }
	 render->camera.Xiw[0][3]=cx;
     render->camera.Xiw[1][3]=cy;
	 render->camera.Xiw[2][3]=cz;
	 
	 for(i=0;i<3;i++)
		 render->camera.Xiw[3][i]=0;

	 render->camera.Xiw[3][3]=1;

	 render->camera.Xiw;
	 GzPushMatrix(render,render->camera.Xiw);
	 GzPushXNorm(render,render->camera.Xiw);
	 
	return GZ_SUCCESS;
}

int GzPushMatrix(GzRender *render, GzMatrix	matrix)
{
	int i,j,k;
	int n,m,p,q;
	//sizeof(array) / sizeof(array[0]);
	float c;
	if(render->matlevel>MATLEVELS)
	  return GZ_FAILURE;
  else
  {
	  
	  render->matlevel++;
		  for(i=0;i<4;i++)
		  for(j=0;j<4;j++)
		  { c=0;
			  if( render->matlevel==0)
				  for(k=0;k<4;k++)
					c+=matrix[k][j];
			  if( render->matlevel>0)
				  for(k=0;k<4;k++)
					c+=render->Ximage[ render->matlevel-1][i][k]*matrix[k][j];
			   render->Ximage[ render->matlevel][i][j]=c;
			   
	 
  }  if(render->matlevel==4||render->matlevel==5||render->matlevel==6)
		  GzPushXNorm(render,matrix);
	return GZ_SUCCESS;
  }
}

int GzPopMatrix(GzRender *render)
{
	 if( render->matlevel<0)
	   return GZ_FAILURE;
   else
   {
	   render->matlevel--;
	   return GZ_SUCCESS;
   }
/*
- pop a matrix off the Ximage stack
- check for stack underflow
*/

}
float dotProd(GzCoord G1,GzCoord G2)
{
	G2[0];
	G2[1];
	G2[2];
	float k=G1[X]*G2[X]+G1[Y]*G2[Y]+G1[Z]*G2[Z];
	return k;
}
int GzNormalize(GzCoord G1)
{
	float dic=sqrt(dotProd(G1,G1));
	G1[X]/=dic;G1[Y]/=dic;G1[Z]/=dic;
	return GZ_SUCCESS;
}
int GzPutAttribute(GzRender	*render, int numAttributes, GzToken	*nameList, 
	GzPointer	*valueList) /* void** valuelist */
{
		if(render != NULL && valueList != NULL  && nameList != NULL)
	{
	int k=0,z=0;
	float* x;
	GzLight km;
	for(int i=0;i<numAttributes;i++)
	{
		if(nameList[i]==GZ_RGB_COLOR)
	 {     x=(float*)valueList[i];
	     for( k=0;k<3;k++)
		 (render)->flatcolor[k]=x[k];
		}
		else if(nameList[i]==GZ_INTERPOLATE)
		{
			(render)->interp_mode=*(int *)valueList[i];
			//(render)->interp_mode=0;
		//(render)->interp_mode=1;
			//(render)->interp_mode=2;


		}
		else if(nameList[i]==GZ_DIRECTIONAL_LIGHT)
		{
			
			km=*(GzLight*)valueList[i];
			
				(render)->lights[render->numlights]=km;
			
			render->numlights++;
			
		}
		else if(nameList[i]==GZ_AMBIENT_LIGHT)
		{
			km=*(GzLight *)valueList[i];
			
				render->ambientlight=km; 
		}
		else if(nameList[i]==GZ_AMBIENT_COEFFICIENT)
		{
			x=*(GzColor *)valueList[i];
			for(k=0;k<3;k++)
			{
				render->Ka[k]=x[k];
			}
		}
		else if(nameList[i]==GZ_DIFFUSE_COEFFICIENT)
		{
			x=*(GzColor *)valueList[i];
			for(k=0;k<3;k++)
				render->Kd[k]=x[k];
		}
		else if(nameList[i]==GZ_SPECULAR_COEFFICIENT)
		{
			x=*(GzColor *)valueList[i];
			for(k=0;k<3;k++)
				render->Ks[k]=x[k];
		}
		else if(nameList[i]==GZ_DISTRIBUTION_COEFFICIENT)
		{
			render->spec=*(float *)valueList[i];
		}
		else if(nameList[i]==GZ_TEXTURE_MAP)	
		{	
		render->tex_fun=(GzTexture) valueList[i];
		render->textureon=1;

		}
		
	}
	return GZ_SUCCESS;}
	else
		return GZ_FAILURE;
	
	return GZ_SUCCESS;
}
float areatriangle(GzCoord p1,GzCoord p2,GzCoord p3)
{
	return abs(0.5*(p1[X]*p2[Y] + p1[Y]*p3[X] + p2[X]*p3[Y] - p2[Y]*p3[X] - p1[Y]*p2[X] - p1[X]*p3[Y])); 
}
int CalculateC(GzCoord *sortedlist,GzCoord normal,GzRender *render,GzColor color)
{			
			int i,j,k;
			float val,val2;
        	GzCoord R[10];
			int jump[10];
			GzColor C,spec,dif;
			GzCoord E={0,0,-1};
			GzNormalize(E);
			GzNormalize(normal);
			normal;
			float RdotE[10];
			for(i=0;i<render->numlights;i++)
			{
				val=dotProd(normal,E);
				val2=dotProd(normal,render->lights[i].direction);
			   if(val>0&&val2>0)jump[i]=2;
			   else if(val<0&&val2<0)
				   {jump[i]=-2;
				    for(j=0;j<3;j++)
					   normal[j]=-1*normal[j];
				   }
			   else {jump[i]=0;continue;}
			   //val2=dotProd(normal,render->lights[i].direction);
			   //val=dotProd(normal,E);
					  //val2=0;
					  for(k=0;k<3;k++)
					{
						R[i][k]=2*val2*normal[k]-render->lights[i].direction[k];
					}
					  GzNormalize(R[i]);
					  val2=dotProd(R[i],E);
					   if(val2<0)val2=0;if(val2>1)val2=1;
					  RdotE[i]=val2;
			} 
					float s,d;
					s=d=0;
					spec[0]=spec[1]=spec[2]=dif[0]=dif[1]=dif[2]=0;
					
					for(i=0;i<render->numlights;i++)
					{
						if(jump[i]==1)continue;
						if(jump[i]==2||jump[i]==-2)
						{
							for(j=0;j<3;j++)
								{
									spec[j]+=render->lights[i].color[j]*pow(RdotE[i],render->spec);
									dif[j]+=render->lights[i].color[j]*dotProd(normal,render->lights[i].direction);
							    }
						}
						
					}
					for(j=0;j<3;j++)
					{
						if(render->textureon!=1)
						{
							spec[j]=render->Ks[j]*spec[j];
									dif[j]=render->Kd[j]*dif[j];
									color[j]=spec[j]+dif[j]+render->Ka[j]*render->ambientlight.color[j];
						}

						if(render->interp_mode==0)
								{	spec[j]=render->Ks[j]*spec[j];
									dif[j]=render->Kd[j]*dif[j];
									color[j]=spec[j]+dif[j]+render->Ka[j]*render->ambientlight.color[j];
						}
						if(render->interp_mode==1)
						{
						color[j]=(spec[j]+dif[j]+render->ambientlight.color[j]);
						
						}
						if(render->interp_mode==2)
						{
							
							spec[j]=render->Ks[j]*spec[j];
									dif[j]=color[j]*dif[j];
									color[j]=spec[j]+dif[j]+color[j]*render->ambientlight.color[j];
						}


					}

				
return GZ_SUCCESS;
}
//MAKE THIS BETTER
int rasterize(GzCoord *sortedlist2,GzCoord *normallist,GzRender *render,GzColor *C,GzTextureIndex *UV)
{
	 int x,y,i,j,si;
	 GzCoord newcolor;
	 float z;
	 float minx,maxx,miny,maxy;
	 float Plane[4];
	 float maxz=(float)MAXINT;
	 GzCoord P;
	 GzCoord sortedlist[3];
	 for(i=0;i<3;i++)
					for(j=0;j<3;j++)
						sortedlist[i][j]=sortedlist2[i][j];

	 for(si=0;si<AAKERNEL_SIZE;si++)
	 {   
		if(si<AAKERNEL_SIZE-2)
		  for(i=0;i<3;i++)
					for(j=0;j<2;j++)
						sortedlist[i][j]=(float)sortedlist2[i][j]-(float)AAFilter[si][j];
				P[0]=P[1]=P[2]=0;
				
				
	 Plane[0]=(sortedlist[V2][Y]-sortedlist[V1][Y])
		     *(sortedlist[V3][Z]-sortedlist[V2][Z])
			 -(sortedlist[V2][Z]-sortedlist[V1][Z])
			 *(sortedlist[V3][Y]-sortedlist[V2][Y]);
	 Plane[1]=(sortedlist[V2][Z]-sortedlist[V1][Z])
		     *(sortedlist[V3][X]-sortedlist[V2][X])
			 -(sortedlist[V2][X]-sortedlist[V1][X])
			 *(sortedlist[V3][Z]-sortedlist[V2][Z]);
	 Plane[2]=(sortedlist[V2][X]-sortedlist[V1][X])
		     *(sortedlist[V3][Y]-sortedlist[V2][Y])
			 -(sortedlist[V2][Y]-sortedlist[V1][Y])
			 *(sortedlist[V3][X]-sortedlist[V2][X]);
	 Plane[3]=-(Plane[0]*sortedlist[V1][X])-(Plane[1]*sortedlist[V1][Y])-(Plane[2]*sortedlist[V1][Z]);
	 
		 P[0]=(normallist[V2][Y]-normallist[V1][Y])
		     *(normallist[V3][Z]-normallist[V2][Z])
			 -(normallist[V2][Z]-normallist[V1][Z])
			 *(normallist[V3][Y]-normallist[V2][Y]);
	 P[1]=(normallist[V2][Z]-normallist[V1][Z])
		     *(normallist[V3][X]-normallist[V2][X])
			 -(normallist[V2][X]-normallist[V1][X])
			 *(normallist[V3][Z]-normallist[V2][Z]);
	 P[2]=(normallist[V2][X]-normallist[V1][X])
		     *(normallist[V3][Y]-normallist[V2][Y])
			 -(normallist[V2][Y]-normallist[V1][Y])
			 *(normallist[V3][X]-normallist[V2][X]);
	 

	 float edges[3];
	 float Area=areatriangle(sortedlist[V1],sortedlist[V2],sortedlist[V3]);
	 minx=floor(min(sortedlist[V1][X],min(sortedlist[V2][X],sortedlist[V3][X])));
	 maxx=ceil(max(sortedlist[V1][X],max(sortedlist[V2][X],sortedlist[V3][X])));
	 for(x=minx;x<=maxx+1;x++)
	 {  if(x<0||x>render->display->xres-1)continue;
		 for(y=floor(sortedlist[V1][Y]);y<=ceil(sortedlist[V3][Y]);y++)
		 {
			 GzCoord val={x,y,1};
			 if(y<0||y>render->display->yres-1)continue;
			 edges[0]=(sortedlist[V2][Y]-sortedlist[V1][Y])*((float)x-sortedlist[V1][X])
				     -(sortedlist[V2][X]-sortedlist[V1][X])*((float)y-sortedlist[V1][Y]);
			 edges[1]=(sortedlist[V3][Y]-sortedlist[V2][Y])*((float)x-sortedlist[V2][X])
				     -(sortedlist[V3][X]-sortedlist[V2][X])*((float)y-sortedlist[V2][Y]);
			  edges[2]=(sortedlist[V1][Y]-sortedlist[V3][Y])*((float)x-sortedlist[V3][X])
				     -(sortedlist[V1][X]-sortedlist[V3][X])*((float)y-sortedlist[V3][Y]);
              if(edges[0]==0||edges[1]==0||edges[2]==0||
				  ((edges[0]<0)&&(edges[1]<0)&&(edges[2]<0))||
				  ((edges[0]>0)&&(edges[1]>0)&&(edges[2]>0)))
			  {
				  int hello=0;
				  if((edges[0]<0)&&(edges[1]<0)&&(edges[2]<0))
					  hello=1;
				  if((edges[0]>0)&&(edges[1]>0)&&(edges[2]>0))
					  hello=2;
				z=(-(Plane[0]*(float)x)-(Plane[1]*(float)y)-Plane[3])/Plane[2];
				float th;
				float areafact[3];
				GzIntensity a,r,g,b;
				GzDepth oldz;
				GzIntensity a1,r1,g1,b1;
				GzDepth oldz1;
				GzCoord val2;
				  GzCoord NewNormal;
				  NewNormal[0]=NewNormal[1]=NewNormal[2]=0;
				val2[0]=val2[1]=val2[2]=0;
				GzCoord Pnew={Plane[2],Plane[1],Plane[0]};
				newcolor[0]=newcolor[1]=newcolor[2]=0;
				
				GzTextureIndex NewUV;
							GzTextureIndex uvx;
						GzColor newtex;
	float Vz;
	render->FinalColor[0]=0;
	render->FinalColor[1]=0;
	render->FinalColor[2]=0;
	switch(render->interp_mode)
				{
					case GZ_FLAT:
					        GzGetDisplay(render->display,x,y,&r,&g,&b,&a,&oldz);
					     	//GzCoord P;
							P[0]=Plane[0];
							P[1]=Plane[1];
							P[2]=Plane[2];
                            						
							GzNormalize(P);

							
							   
							GzMatrix G1;
							NormalizeXnorm(render,&G1);
							GzNormalize(P);
		
							for( i=0;i<1;i++)
							{	
		
								 for(int j=0;j<3;j++)
									 NewNormal[j]=G1[j][X]*normallist[0][X]
													+G1[j][Y]*normallist[0][Y]
													+G1[j][Z]*normallist[0][Z];
								GzNormalize(NewNormal);	
							}
				//GzCoord Pnew={P[2],P[1],P[0]};
							GzNormalize(NewNormal);
							CalculateC(sortedlist,NewNormal,render,newcolor);
							NewUV[0] = (areafact[0]*UV[0][0] + areafact[1]*UV[1][0] + areafact[2]*UV[2][0]) / Area;
						NewUV[1] = (areafact[0]*UV[0][1] + areafact[1]*UV[1][1] + areafact[2]*UV[2][1]) / Area;
						MAXINT-z;
						Vz = (z / ((float)MAXINT - z));
						uvx[0] =NewUV[0] * (Vz + 1);
						uvx[1] = NewUV[1] * (Vz + 1);
							render->tex_fun(uvx[0], uvx[1], newtex); 
							for(i=0;i<3;i++)


							{	newcolor[i]=newtex[i];
								if(newcolor[i]==1.0)
							    float hgrs=1;
								if(newcolor[i]>1.0)
							       newcolor[i]=1;
								 if(newcolor[i]<0.0)
									 newcolor[i]=0;
							}
								if(z<oldz)
							 {
								 
								 GzPutDisplay(render->display,x,y,ctoi(newcolor[0]),ctoi(newcolor[1]),ctoi(newcolor[2]),1,z);

							}
							break;
					             
				case GZ_COLOR:
							GzGetDisplay(render->displayfinal[si],x,y,&r,&g,&b,&a,&oldz);
							if (x==int(sortedlist[0][0])&&y==int(sortedlist[0][1]));
							 th=0;
					        areafact[0]=areatriangle(sortedlist[1],val,sortedlist[2]);
							areafact[1]=areatriangle(sortedlist[0],val,sortedlist[2]);
							areafact[2]=areatriangle(sortedlist[0],val,sortedlist[1]);
							C[0];C[1];C[2];
							if(render->textureon==1)
							{
							z= (areafact[0]*sortedlist[0][Z] + areafact[1]*sortedlist[1][Z] + areafact[2]*sortedlist[2][Z]) /Area;
							NewUV[0] = (areafact[0]*UV[0][0] + areafact[1]*UV[1][0] + areafact[2]*UV[2][0]) / Area;
						NewUV[1] = (areafact[0]*UV[0][1] + areafact[1]*UV[1][1] + areafact[2]*UV[2][1]) / Area;
						MAXINT-z;
						Vz = (z / ((float)MAXINT - z));
						uvx[0] =NewUV[0] * (Vz + 1);
						uvx[1] = NewUV[1] * (Vz + 1);
							render->tex_fun(uvx[0], uvx[1], newtex); 
							}
	                       GzGetDisplay(render->displayfinal[si],x,y,&r1,&g1,&b1,&a1,&oldz1);
							for(i=0;i<3;i++)
							{	for(j=0;j<3;j++)
									newcolor[i]+=areafact[j]*C[j][i];
								newcolor[i]=newcolor[i]/Area;
								if(render->textureon==1){
									newcolor[i]=newtex[i]*newcolor[i];}
								 if(newcolor[i]>1.0)newcolor[i]=1;
								 if(newcolor[i]<0.0)newcolor[i]=0;
							}
							newcolor[0];
							newcolor[1];
							newcolor[2];
							/*render->FinalColor[0]=r1;
							render->FinalColor[1]=g1;
							render->FinalColor[2]=b1;
							render->FinalColor[3]=a1;
							if(si<AAKERNEL_SIZE&&si!=0)
							{
								render->FinalColor[0]+=ctoi(newcolor[0]*AAFilter[si][2]);
							render->FinalColor[1]+=ctoi(newcolor[1]*AAFilter[si][2]);
							render->FinalColor[2]+=ctoi(newcolor[2]*AAFilter[si][2]);
							render->FinalColor[3]+=a1*AAFilter[si][2];
							}
							if(render->current==0)
							{
									render->FinalColor[0]=ctoi(newcolor[0]);
							render->FinalColor[1]=ctoi(newcolor[1]);
							render->FinalColor[2]=ctoi(newcolor[2]);
							render->FinalColor[3]=a1;
							}
							*/
							if(z<oldz)
							 {
								 x;
								 y;
								 GzPutDisplay(render->displayfinal[si],x,y,ctoi(newcolor[0]),ctoi(newcolor[1]),ctoi(newcolor[2]),a,z);
								// GzPutDisplay(render->display,x,y,render->FinalColor[0],render->FinalColor[1],render->FinalColor[2],render->FinalColor[3],z);
							}
							break;
				case GZ_NORMALS:
					GzGetDisplay(render->displayfinal[si],x,y,&r,&g,&b,&a,&oldz);
					        areafact[0]=areatriangle(sortedlist[1],val,sortedlist[2]);
							areafact[1]=areatriangle(sortedlist[0],val,sortedlist[2]);
							areafact[2]=areatriangle(sortedlist[0],val,sortedlist[1]);
							normallist[0];
							normallist[1];
							normallist[2];
							z= (areafact[0]*sortedlist[0][Z] + areafact[1]*sortedlist[1][Z] + areafact[2]*sortedlist[2][Z]) /Area;
							if(render->textureon==1)
							{
							NewUV[0] = (areafact[0]*UV[0][0] + areafact[1]*UV[1][0] + areafact[2]*UV[2][0]) / Area;
						     NewUV[1] = (areafact[0]*UV[0][1] + areafact[1]*UV[1][1] + areafact[2]*UV[2][1]) / Area;
						      Vz = z / (maxz - z);
								uvx[0] =NewUV[0] * (Vz + 1);
								uvx[1] = NewUV[1] * (Vz + 1);
								render->tex_fun(uvx[0], uvx[1], newcolor);
							}
							GzCoord val2;
							
							for(i=0;i<3;i++)
							{	for(j=0;j<3;j++)
									val2[i]+=areafact[j]*normallist[j][i];
								 val2[i]=val2[i]/Area;
								 if(newcolor[i]>1.0)newcolor[i]=1;
							}
							val[0];
							val[1];
							val[2];
							GzNormalize(val2);
						
							CalculateC(sortedlist,val2,render,newcolor);
							for(i=0;i<3;i++)
							{	 
								if(newcolor[i]>1.0)newcolor[i]=1;
								if(newcolor[i]<0.0)newcolor[i]=0;
							}
						
								if(z<oldz)
							 {
								 
								 GzPutDisplay(render->displayfinal[si],x,y,ctoi(newcolor[0]),ctoi(newcolor[1]),ctoi(newcolor[2]),1,z);

							}
							break;
				
				}
			  }
		 }
	 }
   }
   for(x=minx;x<=maxx+1;x++)
	 {  if(x<0||x>render->display->xres-1)continue;
		 for(y=floor(sortedlist[V1][Y]);y<=ceil(sortedlist[V3][Y]);y++)
		 {
			 if(y<0||y>render->display->yres-1)continue;
					GzIntensity r2,r1,g1,a1,g2,b1,b2,a2;
			GzDepth z;
			r1=g1=a1=b1=0;
			 for(i=0;i<AAKERNEL_SIZE;i++)
			 {
				 GzGetDisplay(render->displayfinal[i],x,y,&r2,&g2,&b2,&a2,&z);
				// for(j=0;j<6;j++)
				 //{
					 r1+=AAFilter[i][2]*r2;
					 g1+=AAFilter[i][2]*g2;
					b1+=AAFilter[i][2]*b2;
					a1+=AAFilter[i][2]*a2;
					
		 }
			 GzPutDisplay(render->display,x,y,r1,g1,b1,a1,z);
   }
   }
	 return GZ_SUCCESS;
}
int SwapPoints(GzCoord *sortedlist,GzCoord *normallist,GzTextureIndex *UV)
{
	float temp;
				for(int j=0;j<3;j++)
					 {
						for(int i=0;i<2;i++)
						{	 
							if(ceil(sortedlist[i][1])==ceil(sortedlist[i+1][1]))
							 {
								if(sortedlist[i][0]>sortedlist[i+1][0])
								{
									  temp=sortedlist[i][1];
									  sortedlist[i][1]=sortedlist[i+1][1];
									  sortedlist[i+1][1]=temp;
									  temp=sortedlist[i][0];
									  sortedlist[i][0]=sortedlist[i+1][0];
									  sortedlist[i+1][0]=temp;
									  temp=sortedlist[i][2];
									  sortedlist[i][2]=sortedlist[i+1][2];
									  sortedlist[i+1][2]=temp;
									  temp=normallist[i][1];
									  normallist[i][1]=normallist[i+1][1];
									  normallist[i+1][1]=temp;
								      temp=normallist[i][0];
									  normallist[i][0]=normallist[i+1][0];
									  normallist[i+1][0]=temp;
									  temp=normallist[i][2];
									  normallist[i][2]=normallist[i+1][2];
									  normallist[i+1][2]=temp;
									  temp=UV[i][0];
									  UV[i][0]=UV[i+1][0];
									  UV[i+1][0]=temp;
									  temp=UV[i][1];
									  UV[i][1]=UV[i+1][1];
									  UV[i+1][1]=temp;
				
								}
							}
							else if(sortedlist[i][1]>sortedlist[i+1][1])
								 {
									  temp=sortedlist[i][1];
									  sortedlist[i][1]=sortedlist[i+1][1];
									  sortedlist[i+1][1]=temp;
									  temp=sortedlist[i][0];
									  sortedlist[i][0]=sortedlist[i+1][0];
									  sortedlist[i+1][0]=temp;
									  temp=sortedlist[i][2];
									  sortedlist[i][2]=sortedlist[i+1][2];
									  sortedlist[i+1][2]=temp;
									  temp=normallist[i][1];
									  normallist[i][1]=normallist[i+1][1];
									  normallist[i+1][1]=temp;
									   temp=normallist[i][0];
									  normallist[i][0]=normallist[i+1][0];
									  normallist[i+1][0]=temp;
									   temp=normallist[i][2];
									  normallist[i][2]=normallist[i+1][2];
									  normallist[i+1][2]=temp;
									  temp=UV[i][0];
									  UV[i][0]=UV[i+1][0];
									  UV[i+1][0]=temp;
									  temp=UV[i][1];
									  UV[i][1]=UV[i+1][1];
									  UV[i+1][1]=temp;
								 }
						}
				  }
	return GZ_SUCCESS;
}

int ChangeSignofNormal(GzCoord *normallist)
{   int i,val2change;
	for(i=0;i<3;i++)		
		{
			if(normallist[0][i]>=0&&normallist[1][i]>=0&&normallist[2][i]>=0)continue;
			else if(normallist[0][i]<0&&normallist[1][i]<0&&normallist[2][i]<0)continue;
			else if((normallist[0][i]>=0)^(normallist[1][i]<0))
				val2change=2;
			else if((normallist[2][i]>=0)^(normallist[1][i]<0))
				val2change=0;
			else if((normallist[2][i]>=0)^(normallist[0][i]<0))
				val2change=1;
			normallist[val2change][i]=-1*normallist[val2change][i];
		}
	return GZ_SUCCESS;
}
int GzRenderGourand(GzCoord *sortedlist,GzCoord *normallist,GzRender *render,GzTextureIndex *UV)
{
	 int i;
    GzCoord NewNormal[3];
	GzMatrix G1;
	int val2change;
	NormalizeXnorm(render,&G1);
	//ChangeSignofNormal(normallist);
	for( i=0;i<3;i++)
	{	
		GzNormalize(normallist[i]);
		
         for(int j=0;j<3;j++)
			 NewNormal[i][j]=G1[j][X]*normallist[i][X]
							+G1[j][Y]*normallist[i][Y]
							+G1[j][Z]*normallist[i][Z];
		 
		GzNormalize(NewNormal[i]);	
	}
	
//ChangeSignofNormal(NewNormal);	
		SwapPoints(sortedlist,NewNormal,UV);

	GzColor color[3];
	for(i=0;i<3;i++)
	CalculateC(sortedlist,NewNormal[i],render,color[i]);
	rasterize(sortedlist,NewNormal,render,color,UV);
	return GZ_SUCCESS;
}

int GzRenderPhong(GzCoord *sortedlist,GzCoord *normallist,GzRender *render,GzTextureIndex *UV)
{   int i;
    GzCoord NewNormal[3];
	GzMatrix G1;
	NormalizeXnorm(render,&G1);
	for( i=0;i<3;i++)
	{	
		GzNormalize(normallist[i]);
		
         for(int j=0;j<3;j++)
			 NewNormal[i][j]=G1[j][X]*normallist[i][X]
							+G1[j][Y]*normallist[i][Y]
							+G1[j][Z]*normallist[i][Z];
		GzNormalize(NewNormal[i]);	
	}

			
	SwapPoints(sortedlist,NewNormal,UV);
	GzColor color[3];
	rasterize(sortedlist,NewNormal,render,color,UV);
	return GZ_SUCCESS;
}
int GzRenderFlat(GzCoord *sortedlist,GzCoord *normallist,GzRender *render,GzTextureIndex *UV)
{
	int i ,j,k;
	
	GzColor C[3];
	SwapPoints(sortedlist,normallist,UV);
	rasterize(sortedlist,normallist,render,C,UV);

			 	
return GZ_SUCCESS;			 }

//int GzRenderthisTriangle(Gz
int GzPutTriangle(GzRender	*render, int numParts, GzToken *nameList, GzPointer	*valueList)
/* numParts : how many names and values */
{
	int i,j,k;
	float temp;
	GzCoord newtrianglepoints[3];
	for(i=0;i<numParts;i+2)
		{ 
			if(nameList[i]==GZ_NULL_TOKEN);
				else if(nameList[i]==GZ_POSITION||nameList[i]==GZ_NORMAL||nameList[i]==GZ_TEXTURE_INDEX)
				{
					GzCoord *sortedlist= (GzCoord*)valueList[i]; 

					GzCoord *normallist=(GzCoord*)valueList[i+1];

					int ik,jk;
					GzTextureIndex *UV=(GzTextureIndex *) valueList[i+2];
				
					GzTextureIndex NewUV[3];
					
						
					
					sortedlist[0];sortedlist[1];sortedlist[2];normallist[0];normallist[1];normallist[2];
					 //code
					render->Ximage;
					render->Xnorm;
					 for(int tri=0;tri<3;tri++)
					 {//making 4dmatrix for each point
						 
						float inputcoords[4][1];
							for(i=0;i<3;i++)
								inputcoords[i][0]=sortedlist[tri][i];
							inputcoords[3][0]=1;
							
							float outputcoords[4][1];
							int k;
							float c[4];
							c[0]=c[1]=c[2]=0;
							c[3]=0;
							for(i=0;i<4;i++)
							{
							  for(k=0;k<4;k++)
								{  
									 c[i]=c[i]+render->Ximage[render->matlevel][i][k]*inputcoords[k][0];
							  }
							  outputcoords[i][0]=c[i];
							}
							
								render->Ximage;
								for(i=0;i<4;i++)
									
									{
										outputcoords[i][0]=outputcoords[i][0]/outputcoords[3][0];
										if(i!=3)newtrianglepoints[tri][i]=outputcoords[i][0];
								   }

					 }
					 newtrianglepoints[0];
					 newtrianglepoints[1];
					 newtrianglepoints[2];
					    float Vz;
						float maxz=(float)MAXINT;
					for(ik=0;ik<3;ik++)

						{ 


							Vz= newtrianglepoints[ik][Z] / ((float)MAXINT - newtrianglepoints[ik][Z]);
							NewUV[ik][0]=UV[ik][0]/(Vz+1);
							NewUV[ik][1]=UV[ik][1]/(Vz+1);
						}	
					if(render->interp_mode==GZ_FLAT)GzRenderFlat(newtrianglepoints,normallist,render,NewUV);
					if(render->interp_mode==GZ_COLOR)GzRenderGourand(newtrianglepoints,normallist,render,NewUV);
					if(render->interp_mode==GZ_NORMALS)GzRenderPhong(newtrianglepoints,normallist,render,NewUV);
			}
			
	}
	return GZ_SUCCESS;
}







































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































































