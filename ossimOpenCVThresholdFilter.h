#ifndef ossimOpenCVThresholdFilter_HEADER
#define ossimOpenCVThresholdFilter_HEADER
#include "ossim/plugin/ossimSharedObjectBridge.h"
#include "ossim/base/ossimString.h"


#include "ossim/imaging/ossimImageSourceFilter.h"

#include "cv.h"

class ossimOpenCVThresholdFilter : public ossimImageSourceFilter
{
public:
   ossimOpenCVThresholdFilter(ossimObject* owner=NULL);
   ossimOpenCVThresholdFilter(ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   ossimOpenCVThresholdFilter(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   virtual ~ossimOpenCVThresholdFilter();
   ossimString getShortName()const
      {
         return ossimString("OpenCVThreshold");
      }
   
   ossimString getLongName()const
      {
         return ossimString("OpenCV Threshold Filter");
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

TYPE_DATA
};

#endif
