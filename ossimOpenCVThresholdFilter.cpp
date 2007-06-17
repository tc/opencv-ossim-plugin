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
#include "ossimOpenCVThresholdFilter.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/base/ossimNumericProperty.h>

RTTI_DEF1(ossimOpenCVThresholdFilter, "ossimOpenCVThresholdFilter", ossimImageSourceFilter)

ossimOpenCVThresholdFilter::ossimOpenCVThresholdFilter(ossimObject* owner)
:ossimImageSourceFilter(owner),
theTile(NULL),
theThresh1(0.0),
theThresh2(0.0),
theThresh3(0.0)
{
}

ossimOpenCVThresholdFilter::ossimOpenCVThresholdFilter(ossimImageSource* inputSource,
													   double t1,
													   double t2,
													   double t3)
													   : ossimImageSourceFilter(NULL, inputSource),
													   theTile(NULL),
													   theThresh1(t1),
													   theThresh2(t2),
													   theThresh3(t3)
{
}

ossimOpenCVThresholdFilter::ossimOpenCVThresholdFilter(ossimObject* owner,
													   ossimImageSource* inputSource,
													   double t1,
													   double t2,
													   double t3)
													   : ossimImageSourceFilter(owner, inputSource),
													   theTile(NULL),
													   theThresh1(t1),
													   theThresh2(t2),
													   theThresh3(t3)
{
}

ossimOpenCVThresholdFilter::~ossimOpenCVThresholdFilter()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVThresholdFilter::getTile(const ossimIrect& tileRect,
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

void ossimOpenCVThresholdFilter::initialize()
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

ossimScalarType ossimOpenCVThresholdFilter::getOutputScalarType() const
{
	if(!isSourceEnabled())
	{
		return ossimImageSourceFilter::getOutputScalarType();
	}

	return OSSIM_UCHAR;
}

ossim_uint32 ossimOpenCVThresholdFilter::getNumberOfOutputBands() const
{
	if(!isSourceEnabled())
	{
		return ossimImageSourceFilter::getNumberOfOutputBands();
	}
	return 1;
}

bool ossimOpenCVThresholdFilter::saveState(ossimKeywordlist& kwl,
										   const char* prefix)const
{
	ossimImageSourceFilter::saveState(kwl, prefix);

	kwl.add(prefix,
		"t1",
		theThresh1,
		true);
	kwl.add(prefix,
		"t2",
		theThresh2,
		true);
	kwl.add(prefix,
		"t3",
		theThresh3,
		true);

	return true;
}

bool ossimOpenCVThresholdFilter::loadState(const ossimKeywordlist& kwl,
										   const char* prefix)
{
	ossimImageSourceFilter::loadState(kwl, prefix);

	const char* lookup = kwl.find(prefix, "t1");
	if(lookup)
	{
		theThresh1 = ossimString(lookup).toDouble();
	}
	lookup = kwl.find(prefix, "t2");
	if(lookup)
	{
		theThresh2 = ossimString(lookup).toDouble();
	}
	lookup = kwl.find(prefix, "t3");
	if(lookup)
	{
		theThresh3 = ossimString(lookup).toDouble();
	}
	return true;
}

void ossimOpenCVThresholdFilter::runUcharTransformation(ossimImageData* tile)
{   
	IplImage *input;
	IplImage *output;

	input=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
	output=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
	char* bandSrc[3];//FIXME tile->getNumberOfBands()
	char* bandDest;
	bandDest = static_cast< char*>(theTile->getBuf());

	if(tile->getNumberOfBands() == 1)
	{
		bandSrc[0]  = static_cast< char*>(tile->getBuf(0));
		bandSrc[1]  = static_cast< char*>(tile->getBuf(0));
		bandSrc[2]  = static_cast< char*>(tile->getBuf(0));
	}
	else if(tile->getNumberOfBands() == 2)
	{
		bandSrc[0]  = static_cast< char*>(tile->getBuf(0));
		bandSrc[1]  = static_cast<char*>(tile->getBuf(1));
		bandSrc[2]  = static_cast<char*>(tile->getBuf(1));      
	}
	else if(tile->getNumberOfBands() == 3)
	{
		bandSrc[0]  = static_cast<char*>(tile->getBuf(0));
		bandSrc[1]  = static_cast<char*>(tile->getBuf(1));
		bandSrc[2]  = static_cast<char*>(tile->getBuf(2));      
	}
	input->imageData=bandSrc[0];
	output->imageData=bandDest;

	//long offset;

	//long upperBound = tile->getWidth()*tile->getHeight();

	cvThreshold( input, output, (unsigned char)theThresh1, 255, CV_THRESH_BINARY );

	/*for(offset = 0; offset < upperBound; ++offset)
	{
	long value;

	value = irint(theC1*(bandSrc[0][offset]) +
	theC2*(bandSrc[1][offset]) +
	theC3*(bandSrc[2][offset]));

	value = value<255?value:255;
	value = value>0?value:0;

	bandDest[offset] = value;
	}*/

	theTile->validate();
}

ossimRefPtr<ossimProperty> ossimOpenCVThresholdFilter::getProperty(const ossimString& name)const
{
	if(name == "threshold1")
	{
		ossimNumericProperty* numeric = new ossimNumericProperty(name,
			ossimString::toString(theThresh1),
			0.0, 255.0);
		numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
		numeric->setCacheRefreshBit();
		return numeric;
	}

	if(name == "threshold2")
	{
		ossimNumericProperty* numeric = new ossimNumericProperty(name,
			ossimString::toString(theThresh2),
			0.0, 255.0);
		numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
		numeric->setCacheRefreshBit();
		return numeric;
	}
	if(name == "threshold3")
	{
		ossimNumericProperty* numeric = new ossimNumericProperty(name,
			ossimString::toString(theThresh3),
			0.0, 255.0);
		numeric->setNumericType(ossimNumericProperty::ossimNumericPropertyType_FLOAT64);
		numeric->setCacheRefreshBit();
		return numeric;
	}
	return ossimImageSourceFilter::getProperty(name);
}

void ossimOpenCVThresholdFilter::getPropertyNames(std::vector<ossimString>& propertyNames)const
{
	ossimImageSourceFilter::getPropertyNames(propertyNames);
	propertyNames.push_back("threshold1");
	propertyNames.push_back("threshold2");
	propertyNames.push_back("threshold3");
}

void ossimOpenCVThresholdFilter::setProperty(ossimRefPtr<ossimProperty> property)
{
	if(!property) return;
	ossimString name = property->getName();

	if(name == "threshold1")
	{
		theThresh1 = property->valueToString().toDouble();
	}
	if(name == "threshold2")
	{
		theThresh2 = property->valueToString().toDouble();
	}
	if(name == "threshold3")
	{
		theThresh3 = property->valueToString().toDouble();
	}
}