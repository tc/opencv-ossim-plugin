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

#ifndef ossimOpenCVImageSourceFactory_HEADER
#define ossimOpenCVImageSourceFactory_HEADER
#include "ossim/imaging/ossimImageSourceFactoryBase.h"

class ossimImageFileWriter;
class ossimKeywordlist;
class ossimImageWriterFactory;

class ossimOpenCVImageSourceFactory: public ossimImageSourceFactoryBase
{   
public:
    virtual ~ossimOpenCVImageSourceFactory();
    static ossimOpenCVImageSourceFactory* instance();
    virtual ossimObject* createObject(const ossimString& name)const;
    virtual ossimObject* createObject(const ossimKeywordlist& kwl,
                                      const char* prefix=0)const;
    virtual void getTypeNameList(std::vector<ossimString>& typeList)const;
   
 protected:
    // Hide from use.
    ossimOpenCVImageSourceFactory();
    ossimOpenCVImageSourceFactory(const ossimOpenCVImageSourceFactory&);
    const ossimOpenCVImageSourceFactory& operator=(ossimOpenCVImageSourceFactory&);
 
    static ossimOpenCVImageSourceFactory* theInstance;
 TYPE_DATA};

#endif
