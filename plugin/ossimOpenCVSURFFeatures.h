#ifndef ossimOpenCVSURFFeatures_HEADER
#define ossimOpenCVSURFFeatures_HEADER

#include "ossim/plugin/ossimSharedObjectBridge.h"
#include "ossim/base/ossimString.h"
#include "ossim/imaging/ossimImageSourceFilter.h"
#include <ossim/imaging/ossimAnnotationObject.h>

#include "cv.h"

/**
@brief OpenCV SURF based KeyPoints
** (Description from OpenCV Source)
** 
The function cvExtractSURF finds robust features 
in the image, as described in Bay06 . For each feature
it returns its location, size, orientation and optionally
the descriptor, basic or extended. The function can be
used for object tracking and localization, image stitching etc. 
**/

class ossimOpenCVSURFFeatures : public ossimImageSourceFilter //ossimImageSourceFilter
{
public:
   ossimOpenCVSURFFeatures(ossimObject* owner=NULL);
 
   virtual ~ossimOpenCVSURFFeatures();

   ossimString getShortName()const
      {
         return ossimString("OpenCV SURF Features");
      }
   ossimString getLongName()const
      {
         return ossimString("OpenCV SURF Features Identifier");
      }
  
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect, ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   virtual ossimScalarType getOutputScalarType() const;
   
   ossim_uint32 getNumberOfOutputBands() const;
 
   virtual bool saveState(ossimKeywordlist& kwl, const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl, const char* prefix=0);

   /*
   * Methods to expose thresholds for adjustment through the GUI
   */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;


   std::vector<ossimDpt> theKeyPoints;

protected:
   ossimRefPtr<ossimImageData> theTile;
   void runUcharTransformation(ossimImageData* tile);
   double theHessianThreshold;
   
TYPE_DATA
};

#endif
