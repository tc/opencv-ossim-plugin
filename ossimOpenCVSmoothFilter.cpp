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
#include <ossim/base/ossimRefPtr.h>
#include "ossimOpenCVSmoothFilter.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/base/ossimRefPtr.h>

RTTI_DEF1(ossimOpenCVSmoothFilter, "ossimOpenCVSmoothFilter", ossimImageSourceFilter)

ossimOpenCVSmoothFilter::ossimOpenCVSmoothFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
	param1(3),
	param2(3),
	param3(0),
	param4(0)

{
}

ossimOpenCVSmoothFilter::~ossimOpenCVSmoothFilter()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVSmoothFilter::getTile(const ossimIrect& tileRect,
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

void ossimOpenCVSmoothFilter::initialize()
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

ossimScalarType ossimOpenCVSmoothFilter::getOutputScalarType() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getOutputScalarType();
   }
   
   return OSSIM_UCHAR;
}

ossim_uint32 ossimOpenCVSmoothFilter::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   return 1;
}

bool ossimOpenCVSmoothFilter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,
           "param_1",
           param1,
           true);
   kwl.add(prefix,
           "param_2",
           param2,
           true);
   kwl.add(prefix,
           "param_3",
           param3,
           true);
   
   kwl.add(prefix,
           "param_4",
           param4,
           true);
   return true;
}

bool ossimOpenCVSmoothFilter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "param_1");
   if(lookup)
   {
      param1 = ossimString(lookup).toInt();
   }
   lookup = kwl.find(prefix, "param_2");
   if(lookup)
   {
      param2 = ossimString(lookup).toInt();
   }
   lookup = kwl.find(prefix, "param_3");
   if(lookup)
   {
      param3 = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "param_4");
   if(lookup)
   {
      param4 = ossimString(lookup).toDouble();
   }
   return true;
}

void ossimOpenCVSmoothFilter::runUcharTransformation(ossimImageData* tile)
{   
   IplImage *input;
   IplImage *output;

   input=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
   output=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
   char* bandSrc[3];//FIXME tile->getNumberOfBands()
   char* bandDest;
   
      bandSrc[0]  = static_cast< char*>(tile->getBuf(0));

   input->imageData=bandSrc[0];
   bandDest = static_cast< char*>(theTile->getBuf());
   output->imageData=bandDest;

	//cvSmooth( input, output, CV_MEDIAN,
      //         param1, param2, param3,param4 );
 	cvSmooth( input, output, CV_MEDIAN, 9,9);//, param3,param4 );
 
   theTile->validate();
}
