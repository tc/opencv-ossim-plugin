// Copyright (C) 2000 ImageLinks Inc. 
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
#include "ossimOpenCVGoodFeaturesToTrack.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>
#include <ossim/base/ossimRefPtr.h>

RTTI_DEF1(ossimOpenCVGoodFeaturesToTrack, "ossimOpenCVGoodFeaturesToTrack", ossimImageSourceFilter)

ossimOpenCVGoodFeaturesToTrack::ossimOpenCVGoodFeaturesToTrack(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    quality_level(0.1),
    min_distance(10),
    theC3(1.0/3.0),
	thresh(50)
{
}

ossimOpenCVGoodFeaturesToTrack::ossimOpenCVGoodFeaturesToTrack(ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           double c3)
   : ossimImageSourceFilter(NULL, inputSource),
     theTile(NULL),
     quality_level(c1),
     min_distance(c2),
     theC3(c3),
	thresh(50)
{
}

ossimOpenCVGoodFeaturesToTrack::ossimOpenCVGoodFeaturesToTrack(ossimObject* owner,
                                           ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           double c3)
   : ossimImageSourceFilter(owner, inputSource),
     theTile(NULL),
     quality_level(c1),
     min_distance(c2),
     theC3(c3),
	thresh(50)
{
}

ossimOpenCVGoodFeaturesToTrack::~ossimOpenCVGoodFeaturesToTrack()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVGoodFeaturesToTrack::getTile(const ossimIrect& tileRect,
                                                                ossim_uint32 resLevel)
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getTile(tileRect,
                                             resLevel);
   }
   long w     = tileRect.width();
   long h     = tileRect.height();

   
   if(!theTile.valid()) initialize();
   if(!theTile.valid()) return 0;
   
   if(!theTile.valid()) return 0;
   
   ossimRefPtr<ossimImageData> data = 0;
   if(theInputConnection)
   {
      data  = theInputConnection->getTile(tileRect, resLevel);
   }
   else
   {
      return 0;
   }

   if(!data.valid()) return 0;
   if(data->getDataObjectStatus() == OSSIM_NULL ||
      data->getDataObjectStatus() == OSSIM_EMPTY)
   {
      return 0;
   }

   theTile->setImageRectangle(tileRect);
   theTile->makeBlank();
   
   theTile->setOrigin(tileRect.ul());
   runUcharTransformation(data.get());
   
   return theTile;
   
}

void ossimOpenCVGoodFeaturesToTrack::initialize()
{
   if(theInputConnection)
   {
      theTile = 0;
      
      theTile = new ossimU8ImageData(this,
                                     1,
                                     theInputConnection->getTileWidth(),
                                     theInputConnection->getTileHeight());  
      theTile->initialize();
   }
}

ossimScalarType ossimOpenCVGoodFeaturesToTrack::getOutputScalarType() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getOutputScalarType();
   }
   
   return OSSIM_UCHAR;
}

ossim_uint32 ossimOpenCVGoodFeaturesToTrack::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   return 1;
}

bool ossimOpenCVGoodFeaturesToTrack::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           "quality",
           quality_level,
           true);
   kwl.add(prefix,
           "min_dist",
           min_distance,
           true);
   kwl.add(prefix,
           "c3",
           theC3,
           true);
   
   return true;
}

bool ossimOpenCVGoodFeaturesToTrack::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "quality");
   if(lookup)
   {
      quality_level = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "min_dist");
   if(lookup)
   {
      min_distance = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "c3");
   if(lookup)
   {
      theC3 = ossimString(lookup).toDouble();
   }
   return true;
}

void ossimOpenCVGoodFeaturesToTrack::runUcharTransformation(ossimImageData* tile)
{   
   IplImage *input;
   IplImage *output;

   input=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
   output=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
   char* bandSrc[3];//tile->getNumberOfBands() FIXME
   char* bandDest;
   
      bandSrc[0]  = static_cast< char*>(tile->getBuf(0));

   input->imageData=bandSrc[0];
   bandDest = static_cast< char*>(theTile->getBuf());
   output->imageData=bandDest;
	IplImage* eig_image;
	IplImage* temp;
	int width,height;
	double minVal;
	double maxVal;
	CvPoint minLoc;
	CvPoint maxLoc;
					eig_image =	cvCreateImage( cvGetSize(input), 32, 1 );
				temp=cvCreateImage(cvGetSize(input),32,1);
								CvPoint2D32f corners[100];
				int cornercount=100;
   cvCopy(input,output);
   cvGoodFeaturesToTrack( input, eig_image, temp, corners, &cornercount,
								quality_level, min_distance);
                             //int block_size=3,
                            //int use_harris=0, double k=0.04 );
   for (int i=0;i<cornercount;i++){
					//pts[i]=cvPointFrom32f( corners[i]);
					thePoints.push_back(ossimDpt(corners[i].x,corners[i].y)+tile->getOrigin());         
					cvCircle(output,cvPointFrom32f( corners[i]), 6,cvScalar(255) , 6);//esto se puede quitar
				}

   theTile->validate();
}

