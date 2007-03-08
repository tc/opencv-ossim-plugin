#ifndef ossimOpenCVPyrSegmentation_HEADER
#define ossimOpenCVPyrSegmentation_HEADER
#include "ossim/plugin/ossimSharedObjectBridge.h"
#include <ossim/base/ossimString.h>


#include "ossim/imaging/ossimImageSourceFilter.h"

#include "cv.h"

class ossimOpenCVPyrSegmentation : public ossimImageSourceFilter
{
public:
   ossimOpenCVPyrSegmentation(ossimObject* owner=NULL);
   ossimOpenCVPyrSegmentation(ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   ossimOpenCVPyrSegmentation(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   virtual ~ossimOpenCVPyrSegmentation();
   ossimString getShortName()const
      {
         return ossimString("grey");
      }
   
   ossimString getLongName()const
      {
         return ossimString("rgb to grey scale filter");
      }
   
   virtual ossimRefPtr<ossimImageData> getTile(const ossimIrect& tileRect, ossim_uint32 resLevel=0);
   
   virtual void initialize();
   
   virtual ossimScalarType getOutputScalarType() const;
   
   ossim_uint32 getNumberOfOutputBands() const;
 
   virtual bool saveState(ossimKeywordlist& kwl,
                          const char* prefix=0)const;
   
   /*!
    * Method to the load (recreate) the state of an object from a keyword
    * list.  Return true if ok or false on error.
    */
   virtual bool loadState(const ossimKeywordlist& kwl,
                          const char* prefix=0);

protected:
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   void runUcharTransformation(ossimImageData* tile);
   double theC1;
   double theC2;
   double theC3;
int  threshold1, threshold2;
TYPE_DATA
};

#endif
