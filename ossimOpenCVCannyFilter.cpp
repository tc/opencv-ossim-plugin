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
// Author: Garrett Potts (gpotts@imagelinks)
// Description: A brief description of the contents of the file.
//
//*************************************************************************
// $Id: ossimSharedRgbToGreyFilter.cpp,v 1.10 2005/05/23 13:06:55 gpotts Exp $

#include "ossimOpenCVCannyFilter.h"

#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/base/ossimRefPtr.h>

RTTI_DEF1(ossimOpenCVCannyFilter, "ossimOpenCVCannyFilter", ossimImageSourceFilter)

ossimOpenCVCannyFilter::ossimOpenCVCannyFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theThreshold1(1.0/3.0),
    theThreshold2(1.0/3.0),
    theApertureSize(3)
{
}

ossimOpenCVCannyFilter::ossimOpenCVCannyFilter(ossimImageSource* inputSource,
                                           double threshold1 = 1.0/3.0, 
					   double threshold2 = 1.0/3.0, 
					   int apertureSize = 3)
   : ossimImageSourceFilter(NULL, inputSource),
     theTile(NULL),
     theThreshold1(threshold1),
     theThreshold2(threshold2),
     theApertureSize(apertureSize)
{
}

ossimOpenCVCannyFilter::ossimOpenCVCannyFilter(ossimObject* owner,
                                           ossimImageSource* inputSource,
                                           double threshold1 = 1.0/3.0, 
					   double threshold2 = 1.0/3.0, 
					   int apertureSize = 3)
   : ossimImageSourceFilter(owner, inputSource),
     theTile(NULL),
     theThreshold1(threshold1),
     theThreshold2(threshold2),
     theApertureSize(apertureSize)
{
}

ossimOpenCVCannyFilter::~ossimOpenCVCannyFilter()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVCannyFilter::getTile(const ossimIrect& tileRect, ossim_uint32 resLevel) 
{

	if(!isSourceEnabled())
   	{
	      return ossimImageSourceFilter::getTile(tileRect, resLevel);
	}
	long w     = tileRect.width();
	long h     = tileRect.height();
   
   	if(!theTile.valid()) initialize();
	if(!theTile.valid()) return 0;
  
	ossimRefPtr<ossimImageData> data = 0;
	if(theInputConnection)
	{
		data  = theInputConnection->getTile(tileRect, resLevel);
   	} else {
	      return 0;
   	}

	if(!data.valid()) return 0;
	if(data->getDataObjectStatus() == OSSIM_NULL ||  data->getDataObjectStatus() == OSSIM_EMPTY)
   	{
	     return 0;
   	}

	theTile->setImageRectangle(tileRect);
	theTile->makeBlank();
   
	theTile->setOrigin(tileRect.ul());
	runUcharTransformation(data.get());
   
	printf("Tile (%d,%d) finished!\n",tileRect.ul().x,tileRect.ul().y); 	
   	return theTile;
  
   
}

void ossimOpenCVCannyFilter::initialize()
{
  if(theInputConnection)
  {
      ossimImageSourceFilter::initialize();

      theTile = new ossimU8ImageData(this,
				     theInputConnection->getNumberOfOutputBands(),   
                                     theInputConnection->getTileWidth(),
                                     theInputConnection->getTileHeight());  
      theTile->initialize();
   }
}

ossimScalarType ossimOpenCVCannyFilter::getOutputScalarType() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getOutputScalarType();
   }
   return OSSIM_UCHAR;
}

ossim_uint32 ossimOpenCVCannyFilter::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   return theInputConnection->getNumberOfOutputBands();
}

bool ossimOpenCVCannyFilter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           "theshold1",
           theThreshold1,
           true);
   kwl.add(prefix,
           "threshold2",
           theThreshold2,
           true);
   kwl.add(prefix,
           "aperture_size",
           theApertureSize,
           true);
   
   return true;
}

bool ossimOpenCVCannyFilter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "threshold1");
   if(lookup)
   {
      theThreshold1 = ossimString(lookup).toDouble();
      printf("Read from spec file. threshold1: %f\n",theThreshold1);
   }
   lookup = kwl.find(prefix, "threshold2");
   if(lookup)
   {
      theThreshold2 = ossimString(lookup).toDouble();
      printf("Read from spec file. threshold2: %f\n",theThreshold2);
   }
   lookup = kwl.find(prefix, "aperture_size");
   if(lookup)
   {
      theApertureSize = ossimString(lookup).toInt();
      printf("Read from spec file. aperture_size: %d\n",theApertureSize);
   }
   return true;
}

void ossimOpenCVCannyFilter::runUcharTransformation(ossimImageData* tile)
{   

	IplImage *input;
	IplImage *output;

	char* bSrc;
	char* bDst;

	int nChannels = tile->getNumberOfBands();

	for(int k=0; k<nChannels; k++) {
		printf("Channel %d\n",k);
		input=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
		output=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
		bSrc = static_cast<char*>(tile->getBuf(k));
		input->imageData=bSrc;
		bDst = static_cast<char*>(theTile->getBuf(k));
		output->imageData=bDst;
        	cvCanny(input, output, theThreshold1, theThreshold2, theApertureSize);
		cvReleaseImageHeader(&input);
		cvReleaseImageHeader(&output);
	}

	theTile->validate();   
}
