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
#include "ossimOpenCVErodeFilter.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimNumericProperty.h>

RTTI_DEF1(ossimOpenCVErodeFilter, "ossimOpenCVErodeFilter", ossimImageSourceFilter)

ossimOpenCVErodeFilter::ossimOpenCVErodeFilter(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
	theIter(1)
{
}

ossimOpenCVErodeFilter::~ossimOpenCVErodeFilter()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVErodeFilter::getTile(const ossimIrect& tileRect,
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

void ossimOpenCVErodeFilter::initialize()
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

ossimScalarType ossimOpenCVErodeFilter::getOutputScalarType() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getOutputScalarType();
   }
   
   return OSSIM_UCHAR;
}

ossim_uint32 ossimOpenCVErodeFilter::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   return 1;
}

bool ossimOpenCVErodeFilter::saveState(ossimKeywordlist& kwl,
                                     const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

	kwl.add(prefix,
	"iterations",
	theIter,
	true);
	/*
   kwl.add(prefix,
           "center_y",
           thecenter_y,
           true);
   kwl.add(prefix,
           "M",
           theM,
           true);
   */
   return true;
}

bool ossimOpenCVErodeFilter::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

	const char* lookup = kwl.find(prefix, "iterations");
	if(lookup)
	{
		theIter = ossimString(lookup).toInt();
	}
	/*
   lookup = kwl.find(prefix, "center_y");
   if(lookup)
   {
      thecenter_y = ossimString(lookup).toDouble();
   }
   lookup = kwl.find(prefix, "M");
   if(lookup)
   {
      theM = ossimString(lookup).toDouble();
   }*/
   return true;
}

void ossimOpenCVErodeFilter::runUcharTransformation(ossimImageData* tile)
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

   cvErode( input, output , NULL, theIter);


   theTile->validate();
}

/*
* Methods to expose thresholds for adjustment through the GUI
*/
void ossimOpenCVErodeFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
	if(!property) return;
	ossimString name = property->getName();

	if(name == "iterations")
	{
		theIter = property->valueToString().toInt();
	}
}

ossimRefPtr<ossimProperty> ossimOpenCVErodeFilter::getProperty(const ossimString& name)const
{
	if(name == "iterations")
	{
		ossimNumericProperty* numeric = new ossimNumericProperty(name,
			ossimString::toString(theIter),
			1, 5);
		numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_INT);
		numeric->setCacheRefreshBit();
		return numeric;
	}
	return ossimImageSourceFilter::getProperty(name);
}

void ossimOpenCVErodeFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
	ossimImageSourceFilter::getPropertyNames(propertyNames);
	propertyNames.push_back("iterations");
}
