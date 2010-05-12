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
//  $Id: ossimGdalImageWriterFactory.h,v 1.1.1.1 2005/02/17 21:47:41 dburken Exp $

#ifndef ossimOpenCVPluginFactory_HEADER
#define ossimOpenCVPluginFactory_HEADER

#include "ossim/imaging/ossimImageSourceFactoryBase.h"

class ossimImageFileWriter;
class ossimKeywordlist;
class ossimImageWriterFactory;

class ossimOpenCVPluginFactory: public ossimImageSourceFactoryBase
{   

public:
    virtual ~ossimOpenCVPluginFactory();
    static ossimOpenCVPluginFactory* instance();
    virtual ossimObject* createObject(const ossimString& name)const;
    virtual ossimObject* createObject(const ossimKeywordlist& kwl,const char* prefix=0)const;
    virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
 protected:
    ossimOpenCVPluginFactory();
    ossimOpenCVPluginFactory(const ossimOpenCVPluginFactory&);
    const ossimOpenCVPluginFactory& operator=(ossimOpenCVPluginFactory&);
    static ossimOpenCVPluginFactory* theInstance;

TYPE_DATA
};

#endif
