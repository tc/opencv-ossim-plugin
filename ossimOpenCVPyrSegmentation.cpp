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
#include "ossimOpenCVPyrSegmentation.h"
#include <ossim/imaging/ossimU8ImageData.h>
#include <ossim/base/ossimConstants.h>
#include <ossim/base/ossimCommon.h>
#include <ossim/base/ossimKeywordlist.h>
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/imaging/ossimImageSourceFactoryBase.h>
#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
#include <ossim/base/ossimRefPtr.h>
#include <ossim/imaging/ossimAnnotationPolyObject.h>

RTTI_DEF1(ossimOpenCVPyrSegmentation, "ossimOpenCVPyrSegmentation", ossimImageSourceFilter)

ossimOpenCVPyrSegmentation::ossimOpenCVPyrSegmentation(ossimObject* owner)
   :ossimImageSourceFilter(owner),
    theTile(NULL),
    theLevel(4),
    theThreshold1(255.0),
    theThreshold2(30.0)	
{
}

ossimOpenCVPyrSegmentation::~ossimOpenCVPyrSegmentation()
{
}

ossimRefPtr<ossimImageData> ossimOpenCVPyrSegmentation::getTile(const ossimIrect& tileRect, ossim_uint32 resLevel)
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


void ossimOpenCVPyrSegmentation::initialize()
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

ossimScalarType ossimOpenCVPyrSegmentation::getOutputScalarType() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getOutputScalarType();
   }
   
   return OSSIM_UCHAR;
}

ossim_uint32 ossimOpenCVPyrSegmentation::getNumberOfOutputBands() const
{
   if(!isSourceEnabled())
   {
      return ossimImageSourceFilter::getNumberOfOutputBands();
   }
   return theInputConnection->getNumberOfOutputBands();
}

bool ossimOpenCVPyrSegmentation::saveState(ossimKeywordlist& kwl, const char* prefix)const
{
   ossimImageSourceFilter::saveState(kwl, prefix);

   kwl.add(prefix,"level",theLevel,true);
   kwl.add(prefix,"threshold1",theThreshold1,true);
   kwl.add(prefix,"threshold2",theThreshold2,true); 

   return true;
}

bool ossimOpenCVPyrSegmentation::loadState(const ossimKeywordlist& kwl,
                                     const char* prefix)
{
   ossimImageSourceFilter::loadState(kwl, prefix);

   const char* lookup = kwl.find(prefix, "level");
   if(lookup)
   {
      theLevel = ossimString(lookup).toInt();
      printf("Read from spec file. level: %d\n",theLevel);
   }
   lookup = kwl.find(prefix, "threshold1");
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

   return true;
}

void ossimOpenCVPyrSegmentation::runUcharTransformation(ossimImageData* tile)
{   

	IplImage *input;
	IplImage *output;

	char* bSrc;
	char* bDst;

	CvSeq *comp; // pointer to the output sequence of the segmented components
	CvConnectedComp * cc; // pointer to a segmented component
	int n_comp;  // number of segmented components in the output sequence
	CvMemStorage *storage; 
	int nChannels = tile->getNumberOfBands();

	for(int k=0; k<nChannels; k++) {

		printf("Channel %d\n",k);

		// Pyramids segmentation
		input=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
		output=cvCreateImageHeader(cvSize(tile->getWidth(),tile->getHeight()),8,1);
		bSrc = static_cast<char*>(tile->getBuf(k));
		input->imageData=bSrc;
		bDst = static_cast<char*>(theTile->getBuf(k));
		output->imageData=bDst;
		storage = cvCreateMemStorage (0); // creates a 64K memory storage block 
		cvPyrSegmentation(input, output, storage, &comp, theLevel, theThreshold1, theThreshold2);
		n_comp=comp->total;

		/* FIXME cvPyrSegmentation does not fill 'contour' in CvConnectedComponent struct...
		printf("Number of segments found: %d\n",n_comp);
		int i=0;
		while (i<n_comp)
		{
			printf("Segment %d\n",i);
			cc=(CvConnectedComp*)cvGetSeqElem(comp,i);
			printf("Area: %f\n",cc->area);
			CvScalar s = (CvScalar)cc->value; 
			printf("Value: (%f, %f, %f, %f)\n",s.val[0],s.val[1],s.val[2],s.val[3]);
			printf("Contour: %p\n",cc->contour);
			//for (int j=0; j<currentComp->contour->total;j++)
			//{
			//	printf("(%d,%d) ",((CvPoint*)cvGetSeqElem(currentComp->contour,j))->x,((CvPoint*)cvGetSeqElem(currentComp->contour,j))->y);
			//}
			i++;
		} 
		*/ 
		cvReleaseImageHeader(&input);
		cvReleaseImageHeader(&output);
		cvReleaseMemStorage(&storage);
	}	
  
	theTile->validate(); 



}
