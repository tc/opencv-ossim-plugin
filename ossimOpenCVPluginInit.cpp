//*******************************************************************
// Copyright (C) 2005 David Burken, all rights reserved.
//
// License:  LGPL
// 
// See LICENSE.txt file in the top level directory for more details.
//
// Author:  David Burken
//*******************************************************************
//  $Id: ossimGdalPluginInit.cpp,v 1.6 2005/10/28 13:54:25 gpotts Exp $
#include <gdal.h>
#include <ossim/plugin/ossimSharedObjectBridge.h>
#include <ossimPluginConstants.h>
#include <ossim/base/ossimString.h>
//#include "ossimGdalFactory.h"
//#include "ossimGdalImageWriterFactory.h"
//#include "ossimGdalProjectionFactory.h"
#include "ossimOpenCVImageSourceFactory.h"

#include <ossim/imaging/ossimImageSourceFactoryRegistry.h>
//#include <imaging/factory/ossimImageWriterFactoryRegistry.h>
//#include <projections/factory/ossimProjectionFactoryRegistry.h>


extern "C"
{
   ossimSharedObjectInfo myInfo;
   ossimString theDescription;
   std::vector<ossimString> theObjList;
   const char* getDescription()
   {
      return theDescription.c_str();
   }
   int getNumberOfClassNames()
   {
      return (int)theObjList.size();
   }
   const char* getClassName(int idx)
   {
      if(idx < (int)theObjList.size())
      {
         return theObjList[idx].c_str();
      }
      return 0;
   }
 OSSIM_PLUGINS_DLL   void ossimSharedLibraryInitialize(ossimSharedObjectInfo** info)
   {
      myInfo.getDescription        = getDescription;
      myInfo.getNumberOfClassNames = getNumberOfClassNames;
      myInfo.getClassName          = getClassName;
      *info = &myInfo;
      theDescription = ossimString("OpenCV Library\n\n")+ "Threshold Filter\n\n";
      theObjList.push_back("ossimOpenCVThresholdFilter");
      ossimImageSourceFactoryRegistry::instance()->registerFactory(ossimOpenCVImageSourceFactory::instance());
   }
   
   void ossimSharedLibraryFinalize()
   {
      ossimImageSourceFactoryRegistry::instance()->unregisterFactory(ossimOpenCVImageSourceFactory::instance());
   }
}
