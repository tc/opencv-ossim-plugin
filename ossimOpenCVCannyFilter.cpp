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
#include "ossimOpenCVCannyFilter.h"
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
    theC1(1.0),
    theC2(3.0),
    theC3(3)
{
}

ossimOpenCVCannyFilter::ossimOpenCVCannyFilter(ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           int c3)
   : ossimImageSourceFilter(NULL, inputSource),
     theTile(NULL),
     theC1(c1),
     theC2(c2),
     theC3(c3)
{
}

ossimOpenCVCannyFilter::ossimOpenCVCannyFilter(ossimObject* owner,
                                           ossimImageSource* inputSource,
                                           double c1,
                                           double c2,
                                           int c3)
   : ossimImageSourceFilter(owner, inputSource),
     theTile(NULL),
     theC1(c1),
     theC2(c2),
     theC3(c3)
{
}

ossimOpenCVCannyFilter::~ossimOpenCVCannyFilter()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVCannyFilter::getTile(const ossimIrect& tileRect,
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

void ossimOpenCVCannyFilter::initialize()
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
   return 1;
}

bool ossimOpenCVCannyFilter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

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

bool ossimOpenCVCannyFilter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

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
      theC3 = ossimString(lookup).toInt();
   }
   return true;
}

void ossimOpenCVCannyFilter::runUcharTransformation(ossimImageData* tile)
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

        //cvCanny( input,output, theC1,theC2, theC3 );
cvCanny( input,output, 1,1,3 );

   theTile->validate();
}
