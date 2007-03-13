//*******************************************************************
// Copyright (C) 2000 ImageLinks Inc.
//
// License:  LGPL
//
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  Garrett Potts
//
//*******************************************************************
//  $Id: ossimGdalImageWriterFactory.cpp,v 1.5 2005/10/28 13:54:25 gpotts Exp $

#include "ossimOpenCVImageSourceFactory.h"
#include "ossimOpenCVThresholdFilter.h"
#include "ossimOpenCVPyrSegmentation.h"
#include "ossimOpenCVSobelFilter.h"
#include "ossimOpenCVCannyFilter.h"
#include "ossimOpenCVErodeFilter.h"
#include "ossimOpenCVDilateFilter.h"
#include "ossimOpenCVSmoothFilter.h"
#include "ossimOpenCVLogPolarFilter.h"
#include "ossimOpenCVSquaresFilter.h"
#include "ossimOpenCVGoodFeaturesToTrack.h"
#include <ossim/base/ossimKeywordNames.h>
#include <ossim/base/ossimObjectFactoryRegistry.h>
#include <ossim/base/ossimTrace.h>

RTTI_DEF1(ossimOpenCVImageSourceFactory, "ossimOpenCVImageSourceFactory", ossimImageSourceFactoryBase);

static ossimTrace traceDebug("ossimOpenCVImageSourceFactory:debug");

ossimOpenCVImageSourceFactory* ossimOpenCVImageSourceFactory::theInstance=NULL;
 
ossimOpenCVImageSourceFactory::~ossimOpenCVImageSourceFactory()
{
   theInstance = NULL;
   ossimObjectFactoryRegistry::instance()->unregisterFactory(this);
}
ossimOpenCVImageSourceFactory* ossimOpenCVImageSourceFactory::instance()
{
   if(!theInstance)
   {
      theInstance = new ossimOpenCVImageSourceFactory;
   }

   return theInstance;
}
 
ossimObject* ossimOpenCVImageSourceFactory::createObject(const ossimString& name)const
{

  // lets do the filters first
   if( name == STATIC_TYPE_NAME(ossimOpenCVThresholdFilter))
   {
      return new ossimOpenCVThresholdFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVPyrSegmentation))
   {
      return new ossimOpenCVPyrSegmentation;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVSobelFilter))
   {
      return new ossimOpenCVSobelFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVSquaresFilter))
   {
      return new ossimOpenCVSquaresFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVCannyFilter))
   {
      return new ossimOpenCVCannyFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVDilateFilter))
   {
      return new ossimOpenCVDilateFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVErodeFilter))
   {
      return new ossimOpenCVErodeFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVSmoothFilter))
   {
      return new ossimOpenCVSmoothFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVLogPolarFilter))
   {
      return new ossimOpenCVLogPolarFilter;
   }
   else if(name == STATIC_TYPE_NAME(ossimOpenCVGoodFeaturesToTrack))
   {
      return new ossimOpenCVGoodFeaturesToTrack;
   }

   return NULL;
}

ossimObject* ossimOpenCVImageSourceFactory::createObject(const ossimKeywordlist& kwl,
                                                   const char* prefix)const
{
   static const char* MODULE = "ossimOpenCVImageSourceFactory::createSource";
   
   ossimString copyPrefix = prefix;
   ossimObject* result = NULL;
   
   if(traceDebug())
   {
      CLOG << "looking up type keyword for prefix = " << copyPrefix << endl;
   }

   const char* lookup = kwl.find(copyPrefix, "type");
   if(lookup)
   {
       ossimString name = lookup;
       result           = createObject(name);
       
       if(result)
       {
         if(traceDebug())
         {
             CLOG << "found source " << result->getClassName() << " now loading state" << endl;
          }
          result->loadState(kwl, copyPrefix.c_str());
       }
       else
       {
         if(traceDebug())
         {
            CLOG << "type not found " << lookup << endl;
         }
      }
   }
   else
   {
       if(traceDebug())
       {
          CLOG << "type keyword not found" << endl;
       }
    }
    return result;
}

void ossimOpenCVImageSourceFactory::getTypeNameList(std::vector<ossimString>& typeList)const
{
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVThresholdFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVPyrSegmentation));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVSobelFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVSquaresFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVCannyFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVDilateFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVErodeFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVSmoothFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVLogPolarFilter));
   typeList.push_back(STATIC_TYPE_NAME(ossimOpenCVGoodFeaturesToTrack));
   /*typeList.push_back(STATIC_TYPE_NAME(ossimBlendMosaic));*/
}
 
// Hide from use...
ossimOpenCVImageSourceFactory::ossimOpenCVImageSourceFactory()
   :ossimImageSourceFactoryBase()
{}
 
ossimOpenCVImageSourceFactory::ossimOpenCVImageSourceFactory(const ossimOpenCVImageSourceFactory&)
   :ossimImageSourceFactoryBase()
{}
 
const ossimOpenCVImageSourceFactory& ossimOpenCVImageSourceFactory::operator=(ossimOpenCVImageSourceFactory&)
{
   return *this;
}
