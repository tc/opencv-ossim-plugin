// 
// OSSIM is free software; you can redistribute it and/or
// modify it under the terms of the GNU General Public License 
// as published by the Free Software Foundation.
//
// This software is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. 
//
// You should have received a copy of the GNU General Public License
// along with this software. If not, write to the Free Software 
// Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-
// 1307, USA.
//
// See the GPL in the COPYING.GPL file for more details.
//
// Author: Jorge Artieda (jatrigueros at argongra dot com)
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimSharedRgbToGreyFilter.cpp,v 1.10 2005/05/23 13:06:55 gpotts Exp $
#include <ossim/base/ossimRefPtr.h>
#include "ossimOpenCVRegisterFilter.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/base/ossimRefPtr.h>

RTTI_DEF1(ossimOpenCVRegisterFilter, "ossimOpenCVRegisterFilter", ossimImageCombiner)

ossimOpenCVRegisterFilter::ossimOpenCVRegisterFilter(ossimObject* owner)
   :ossimImageCombiner(owner,
   2,
   0,
   false,
   false),
    theTile(NULL),
    theC1(1.0/3.0),
    theC2(1.0/3.0),
    theC3(1.0/3.0),
	thresh(50)
{
	printf("ckacka de la vacka konstruktor\n");
	cvNamedWindow( "ir", 1 );
	cvNamedWindow( "vis", 1 );
}



ossimOpenCVRegisterFilter::~ossimOpenCVRegisterFilter()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVRegisterFilter::getTile(const ossimIrect& tileRect,
                                                                ossim_uint32 resLevel)
{
	printf("ckacka de la vacka\n");
   //if(!isSourceEnabled())
   //{
   //   return ossimImageSourceFilter::getTile(tileRect,
   //                                          resLevel);
   //}
   //long w     = tileRect.width();
   //long h     = tileRect.height();
 
   //
   //if(!theTile.valid()) initialize();
   //if(!theTile.valid()) return 0;
   //
   //if(!theTile.valid()) return 0;
   //
   //ossimRefPtr<ossimImageData> data = 0;
   //if(theInputConnection)
   //{
   //   data  = theInputConnection->getTile(tileRect, resLevel);
   //}
   //else
   //{
   //   return 0;
   //}
   //ossimImageSource* left  = PTR_CAST(ossimImageSource, getInput(0));
   //ossimImageSource* right = PTR_CAST(ossimImageSource, getInput(1));
   //double v;
   //if (left && right)
   //{
   //   ossimRefPtr<ossimImageData> leftD  = left->getTile(tileRect, resLevel);
   //   ossimRefPtr<ossimImageData> rightD = right->getTile(tileRect, resLevel);
   //}
   //if(!data.valid()) return 0;
   //if(data->getDataObjectStatus() == OSSIM_NULL ||
   //   data->getDataObjectStatus() == OSSIM_EMPTY)
   //{
   //   return 0;
   //}

   //theTile->setImageRectangle(tileRect);
   //theTile->makeBlank();
   //
   //theTile->setOrigin(tileRect.ul());
   //runUcharTransformation(data.get());
   //
   //return theTile;
	if ( (!theTile) )
	{
		initialize();
	}


	long w = tileRect.width();
	long h = tileRect.height();
	long tw = theTile->getWidth();
	long th = theTile->getHeight();

	theTile->setImageRectangle(tileRect);
    theTile->setOrigin(tileRect.ul());
	if(w*h != tw*th)
	{
		theTile->initialize();
	}
	else
	{
		theTile->makeBlank();
	}

	//switchTypeRun(rect, resLevel);
   ossimImageSource* left  = PTR_CAST(ossimImageSource, getInput(0));
   ossimImageSource* right = PTR_CAST(ossimImageSource, getInput(1));
   double v;
   if (left && right)
   {
      ossimRefPtr<ossimImageData> leftD  = left->getTile(tileRect, resLevel);
      ossimRefPtr<ossimImageData> rightD = right->getTile(tileRect, resLevel);
	  printf("ckacka de la vacka pretransform\n");
	     if (leftD.get() && rightD.get())
   {
	  runUcharTransformation(leftD.get(),rightD.get()); 
   }else {
      printf("ERROR no se ejecuta uchar transform porquen o hay imagen\n");
   }
	  printf("ckacka de la vacka posttransform\n");
   }
	return theTile;
}

void ossimOpenCVRegisterFilter::initialize()
{
   ossimImageCombiner::initialize();
   theTile = ossimImageDataFactory::instance()->create(this, this);
   theTile->initialize();  //TBD: change tile creation so it isn't within initialize 
   /*
   if(theInputConnection)
   {
      theTile = 0;
      
      theTile = new ossimU8ImageData(this,
                                     1,
                                     theInputConnection->getTileWidth(),
                                     theInputConnection->getTileHeight());  
      theTile->initialize();
   }*/
}

ossimScalarType ossimOpenCVRegisterFilter::getOutputScalarType() const
{
   if(!isSourceEnabled())
   {
      return ossimImageCombiner::getOutputScalarType();
   }
   
   return OSSIM_UCHAR;
}

ossim_uint32 ossimOpenCVRegisterFilter::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageCombiner::getNumberOfOutputBands();
   }
   return 1;
}

bool ossimOpenCVRegisterFilter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageCombiner::saveState(kwl, prefix);

   kwl.add(prefix,
           "c1",
           theC1,
           true);
   kwl.add(prefix,
           "c2",
           theC2,
           true);
   kwl.add(prefix,
           "c3",
           theC3,
           true);
   
   return true;
}

bool ossimOpenCVRegisterFilter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageCombiner::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "c1");
   if(lookup)
   {
      theC1 = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "c2");
   if(lookup)
   {
      theC2 = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "c3");
   if(lookup)
   {
      theC3 = ossimString(lookup).toDouble();
   }
   return true;
}

void ossimOpenCVRegisterFilter::runUcharTransformation(ossimImageData* left,ossimImageData* right)
{   
   IplImage *Iplleft;
   IplImage *Iplright;
   IplImage *Iploutput;
   printf("ckacka de la vacka before leftright\n");
   if ((left) && (right) && (left->getDataObjectStatus() != OSSIM_EMPTY) && (right->getDataObjectStatus() != OSSIM_EMPTY))
   {
   printf("ckacka de la vacka before imagescreated\n");
   Iplleft=cvCreateImageHeader(cvSize(left->getWidth(),left->getHeight()),8,1);
   printf("ckacka de la vacka left created\n");
   cout<<"right pointer"<<right<<endl;
   Iplright=cvCreateImageHeader(cvSize(right->getWidth(),right->getHeight()),8,1);
   printf("ckacka de la vacka rightcreated\n");
   Iploutput=cvCreateImageHeader(cvSize(left->getWidth(),left->getHeight()),8,1);
   
   printf("ckacka de la vacka imagescreated\n");
   //char* bandSrc[3];//tile->getNumberOfBands() FIXME
   //char* bandDest;
   
 /*  bandSrc[0]  = static_cast< char*>(tile->getBuf(0));

   input->imageData=bandSrc[0];*/
  // bandDest = static_cast< char*>(theTile->getBuf());
   
	   ossim_uint32 upperBound = theTile->getWidth() * theTile->getHeight();
	   printf("ckacka de la vacka upperbound\n");
	   char* leftBand      = static_cast<char*>(left->getBuf(0));
	   printf("ckacka de la vacka punteros left asignados\n");
	   char* rightBand     = static_cast<char*>(right->getBuf(0));
	   printf("ckacka de la vacka punteros right asignados\n");
	   char* destBand      = static_cast<char*>(theTile->getBuf());
	   printf("ckacka de la vacka punteros dest asignados\n");
	   if(leftBand&&rightBand)
	   {
		   Iplleft->imageData=leftBand;
		   Iplright->imageData=rightBand;
		   Iploutput->imageData=destBand;
		   printf("ckacka de la vacka prematch\n");
		   match(Iplleft,Iplright,Iploutput);
		   printf("ckacka de la vacka postmatch\n");
	   }
	   printf("ckacka de la vacka postlefrighif\n");
   }else
   {
		cout<< "error (algunaimagen vacía)"<<left<< " "<< right <<" "<<(left->getDataObjectStatus() != OSSIM_EMPTY) <<" "<< (right->getDataObjectStatus() != OSSIM_EMPTY)<<endl;
   }
printf("ckacka de la vacka postlerightleftrighif\n");
  // storage = cvCreateMemStorage(0);
 // cvCopy( Iplleft,Iploutput);
  //drawSquares( output, findSquares4( input, storage ) );
  //cvClearMemStorage( storage );
   theTile->validate();


}


int  ossimOpenCVRegisterFilter::match(IplImage *vis, IplImage *ir,IplImage *irdeformada)
{
	CvMat** model;
	CvMat* modeltmp;
	model=&modeltmp;
	IplImage *rec_ir, *rec_vis;
	/*IplImage *ss,*hi,*d,*v;
	IplImage *ss_vis,*hi_vis,*d_vis,*v_vis;
	CvMat *ld,*hd,*hr,*lr;
	IplImage *zero;
	wavefilter("haar", &ld, &hd, &lr, &hr);
    wavefast ( ir, ld,hd, &ss, &hi, &d, &v);
	wavefast ( vis, ld,hd, &ss_vis, &hi_vis, &d_vis, &v_vis);
	zero =cvCreateImage(cvGetSize(ss), 8,1);
	cvSetZero(zero);
	rec_ir=waveback(lr,hr,zero,hi,zero,v);
    rec_vis=waveback(lr,hr,zero,hi_vis,zero,v_vis);
	cvReleaseImage(&zero);
	cvReleaseMat(&ld);
	cvReleaseMat(&hd);
	cvReleaseMat(&lr);
	cvReleaseMat(&hr);*/
printf("match 1\n");
	
		IplImage*irgrey;
		IplImage *irborder;
		irgrey=cvCreateImage(cvGetSize(ir),8,1);
		rec_ir=cvCreateImage(cvGetSize(ir),8,1);
		rec_vis=cvCreateImage(cvGetSize(ir),8,1);
		irborder=cvCreateImage(cvGetSize(ir),IPL_DEPTH_16S ,1);
		
        //cvSmooth( ir, irgrey,CV_BLUR,3,3);
		cvSobel( ir, irborder, 1, 1, 3 );
		cvConvertScale(irborder,rec_ir,7,0);
		cvReleaseImage(&irgrey);
		cvReleaseImage(&irborder);

		printf("match 2\n");

		IplImage *visborder;
		visborder=cvCreateImage(cvGetSize(vis),IPL_DEPTH_16S,1);
	    cvSobel( vis, visborder, 1, 1, 3 );
		cvConvertScale(visborder,rec_vis,7,0);
		cvReleaseImage(&visborder);
	
	//aqui va la recursion
	//TODO : RECURSION
	//IplImage *result_vis;
	//IplImage *result_ir;
		*model = cvCreateMat(2,3,CV_32F);

		cvmSet(*model,0,0,1);
		cvmSet(*model,0,1,0);
		cvmSet(*model,0,2,0);

		cvmSet(*model,1,0,0);
		cvmSet(*model,1,1,1);
		cvmSet(*model,1,2,0);
	//buscar puntos i hacer match
printf("match 3\n");
		IplImage* patron;
		IplImage* corr;
		IplImage* visframe;

		IplImage* zonabusca;

		IplImage* eig_image;
		IplImage* temp;
		int width,height;
		double minVal;
		double maxVal;
		CvPoint minLoc;
		CvPoint maxLoc;

		width=20;
		height=20;
		
		visframe=vis;
		int ancho, alto;
		printf("match 4\n");	
		zonabusca=cvCreateImage( cvSize( 200, 200), 8, 1 );//FIXME esto devería ser algo adaptable
		patron=cvCreateImage(cvSize( width, height ),8,1);
			
		ancho= cvGetSize(zonabusca).width-width +1;
		alto = cvGetSize(zonabusca).height-height+1;
		corr = cvCreateImage( cvSize( ancho, alto), 32, 1 );
		
		eig_image =	cvCreateImage( cvGetSize(vis), 32, 1 );
		temp=cvCreateImage(cvGetSize(vis),32,1);
		
		CvPoint2D32f corners[100];
		int cornercount=100;
		CvPoint pts[100];
		CvPoint pts_n[100];
		cvGoodFeaturesToTrack(vis,eig_image,temp,corners,&cornercount,0.1, (int)(cvGetSize(vis).width/20), NULL);
		for (int i=0;i<cornercount;i++){
			if(cvGetReal2D( vis, corners[i].x,corners[i].y )!=255){
				pts[i]=cvPointFrom32f( corners[i]);
			}
		}
printf("match 5\n");
		CvPoint tmppt;
		CvPoint busca_en_pt;
		int quitados =0;
		int encontrados=0;
		int quitados_en_negro=0;
		int quitados_negativos=0;
		
		for (int i=0;i<cornercount;i++){
			//TODO TRANSFORMAR LA ZONA DE BUSQUEDA
			/*FIXME HAY QUE USAR LA TRANSFORMADA INVERSA...	*/
			busca_en_pt.x=pts[i].x-2*(int)cvmGet(*model,0,2);
			busca_en_pt.y=pts[i].y-2*(int)cvmGet(*model,1,2);
			
			cvGetRectSubPix( ir, zonabusca,cvPointTo32f(busca_en_pt));//imagen original
			cvGetRectSubPix( vis,patron ,cvPointTo32f(pts[i]) );
			cvMatchTemplate( zonabusca, patron,corr, CV_TM_CCORR_NORMED );
			cvMinMaxLoc( corr, &minVal, &maxVal, &minLoc, &maxLoc, 0 );
			pts_n[i].x=maxLoc.x+busca_en_pt.x-cvGetSize(corr).width/2;
			pts_n[i].y=maxLoc.y+busca_en_pt.y-cvGetSize(corr).height/2;
		
			cvGetRectSubPix( rec_ir, zonabusca,cvPointTo32f(busca_en_pt ));//reconstruccion o borde
			cvGetRectSubPix( rec_vis, patron,cvPointTo32f(pts[i]) );
			cvMatchTemplate( zonabusca, patron,corr, CV_TM_CCORR_NORMED );
			cvMinMaxLoc( corr, &minVal, &maxVal, &minLoc, &maxLoc, 0 );
			tmppt.x=maxLoc.x+busca_en_pt.x-cvGetSize(corr).width/2;
			tmppt.y=maxLoc.y+busca_en_pt.y-cvGetSize(corr).height/2;
            
            encontrados++;
            cout<<"x : "<<pts_n[i].x<<" y : "<<pts_n[i].y;
            cout<<" xb: "<<tmppt.x<<" yb: "<<tmppt.y<<" dx: "<<tmppt.x-pts_n[i].x<<" dy: "<<tmppt.y - pts_n[i].y <<endl;
            
			if(!(pts_n[i].x<tmppt.x+5 &&//FIXME APLICAR LA WAVELET TIENE UN DESPLAZAMIENTO
				pts_n[i].x>tmppt.x-5 &&
				pts_n[i].y<tmppt.y+5 &&
				pts_n[i].y>tmppt.y-5))
			{
				pts_n[i].x=0;
				pts_n[i].y=0;					
				pts[i].x=0;
				pts[i].y=0;
				quitados++;
			} else if( pts_n[i].x<0||pts_n[i].y < 0||pts_n[i].y >400 ||pts_n[i].x>400)
            {
                pts_n[i].x=0;
				pts_n[i].y=0;					
				pts[i].x=0;
				pts[i].y=0;   
                quitados_negativos++;     
            }
            else if(cvGetReal2D( ir, pts_n[i].x,pts_n[i].y) ==0)
            {
            
             	pts_n[i].x=0;
				pts_n[i].y=0;					
				pts[i].x=0;
				pts[i].y=0;   
                quitados_en_negro++;            
            }
		}
printf("match 6\n");
int pointcount=0;
for (int i =0;i<cornercount;i++)
{
	if (pts_n[i].x!=0&&
		pts_n[i].y!=0&&
		pts[i].x!=0&&
		pts[i].y!=0)
	{
		pts[pointcount].x=pts[i].x;
		pts[pointcount].y=pts[i].y;
		pts_n[pointcount].x=pts_n[i].x;
		pts_n[pointcount].y=pts_n[i].y;
		pointcount++;
	}
}
cout<<"quitados: "<<quitados<<endl;
cout<<"quitados_en_negro: "<<quitados_en_negro<<endl;
cout<<"quitados_negativos: "<<quitados_negativos<<endl;
cout<<"encontrados: "<<encontrados<<endl;
cout<<"resultantes: "<<pointcount<<endl;

printf("match7\n");
for (int i=0;i<cornercount;i++){
    cvCircle(ir,pts_n[i], 6,cvScalar(255) , 6);//esto se puede quitar
	cvCircle(vis,pts[i], 6,cvScalar(255) , 6);//esto se puede quitar
}
printf("match7b\n");
CvMat *xvis;
CvMat *yvis;
CvMat *xir;
CvMat *yir;
CvMat *xyir;
CvMat *param;
//CvMat *warpmat;
if(pointcount>3)
{
	printf("poincount>2\n");

	xvis=cvCreateMat( pointcount, 1, CV_32FC1 );
	yvis=cvCreateMat( pointcount, 1, CV_32FC1 );
	xir=cvCreateMat( pointcount, 1, CV_32FC1 );
	yir=cvCreateMat( pointcount, 1, CV_32FC1 );
	xyir=cvCreateMat( pointcount, 3, CV_32FC1 );
	printf("poincount>2 1\n");
	param=cvCreateMat( 3, 1, CV_32FC1 );
	//warpmat=cvCreateMat(2,3,CV_32FC1);
	FILE *f ;
	f=fopen("kk.txt","a");

	for (int i =0;i<pointcount;i++)
	{
        printf("poincount>2 2\n");
		cvmSet(xvis,i,0,pts[i].x);
		cvmSet(yvis,i,0,pts[i].y);
		cvmSet(xir,i,0,pts_n[i].x);
		cvmSet(yir,i,0,pts_n[i].y);
		cvmSet(xyir,i,0,pts_n[i].x);
		cvmSet(xyir,i,1,pts_n[i].y);
		cvmSet(xyir,i,2,1.);
		ossimDpt imPoint(pts[i].x +theTile->getOrigin().x,pts[i].y +theTile->getOrigin().y);
		ossimGpt gpt;
		transform->getImageProjection ()->lineSampleToWorld(imPoint, gpt);
	  	ossimDpt imPoint2(pts_n[i].x +theTile->getOrigin().x,pts_n[i].y +theTile->getOrigin().y);
		ossimGpt gpt2;
		ossimDpt imPoint3;
		transform2->getImageProjection ()->lineSampleToWorld(imPoint2, gpt2);
		transform2->viewToImage(imPoint2,imPoint3);
	  	
        ossimDpt imPoint4;
		transform->viewToImage(imPoint,imPoint4);
		
		cout<<"x: "<<pts[i].x<<" y: "<<pts[i].y<<" xgeo "<<gpt.lond()<<" ygeo "<<gpt.latd()<<"gpt2"<<gpt2<<" dpt3 "<<imPoint3<< " dpt4 "<<imPoint4<<endl;
		fprintf(f,"%d %d %f %f %d %d \n",imPoint4.x,imPoint4.y,gpt.lond(),gpt.latd(), imPoint3.x,imPoint3.y);
		printf("poincount>2 3\n");
			
	}
	fclose(f);
	cvSolve(xyir,xvis,param,CV_SVD);
	cvmSet(*model,0,0,cvmGet(param,0,0));
	cvmSet(*model,0,1,cvmGet(param,1,0));	
	cvmSet(*model,0,2,cvmGet(param,2,0));
	cvSolve(xyir,yvis,param,CV_SVD);
	cvmSet(*model,1,0,cvmGet(param,0,0));
	cvmSet(*model,1,1,cvmGet(param,1,0));	
	cvmSet(*model,1,2,cvmGet(param,2,0));
	cout<<cvmGet(*model,0,0)<<" "<<cvmGet(*model,0,1)<<" "<<cvmGet(*model,0,2)<<" "<<endl;
	cout<<cvmGet(*model,1,0)<<" "<<cvmGet(*model,1,1)<<" "<<cvmGet(*model,1,2)<<" "<<endl;
	printf("match7bis\n");
	
	cvReleaseMat(&xvis);
	cvReleaseMat(&yvis);	
	cvReleaseMat(&xir);	
	cvReleaseMat(&yir);	
	cvReleaseMat(&xyir);	
	cvReleaseMat(&param);
}else{
    cvmSet(*model,0,2,2*(int)cvmGet(*model,0,2));
    cvmSet(*model,1,2,2*(int)cvmGet(*model,1,2));
    
    cvmSet(*model,0,0,cvmGet(*model,0,0));
    cvmSet(*model,0,1,cvmGet(*model,0,1));
    cvmSet(*model,1,0,cvmGet(*model,1,0));
    cvmSet(*model,1,1,cvmGet(*model,1,1));
    printf("ERROR no hay suficientespuntos usando modelo identidad\n");
}
//*model=warpmat;


printf("match 8\n");
cvReleaseImage( &eig_image );
cvReleaseImage( &temp );
cvReleaseImage( &corr );
cvReleaseImage( &patron );
cvReleaseImage( &zonabusca );
printf("match 8bis\n");
/*cvReleaseImage( & ss);
cvReleaseImage( & hi);
cvReleaseImage( & d);
cvReleaseImage( & v);

cvReleaseImage( & ss_vis);
cvReleaseImage( & hi_vis);
cvReleaseImage( & d_vis);
cvReleaseImage( & v_vis);*/
cvShowImage( "ir", rec_ir );
cvShowImage( "vis", rec_vis );
cvReleaseImage( &rec_ir);
cvReleaseImage( &rec_vis);
//cvReleaseImage( &result_vis);
//cvReleaseImage( &result_ir);
printf("match 8bisbis\n");
//irdeformada=cvCreateImageHeader(cvGetSize(vis),8,1);
printf("match 8bis bis bis\n");
//	cvConvertScale(ir,ir,40,0);

cvWaitKey(0);
cvWarpAffine( ir, irdeformada, *model );

printf("match 9\n");
	return 0;
}
