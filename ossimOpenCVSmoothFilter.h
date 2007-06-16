#ifndef ossimOpenCVSmoothFilter_HEADER
#define ossimOpenCVSmoothFilter_HEADER
#include "ossim/plugin/ossimSharedObjectBridge.h"
#include "ossim/base/ossimString.h"


#include "ossim/imaging/ossimImageSourceFilter.h"

#include "cv.h"

class ossimOpenCVSmoothFilter : public ossimImageSourceFilter
{
public:
   ossimOpenCVSmoothFilter(ossimObject* owner=NULL);
   ossimOpenCVSmoothFilter(ossimImageSource* inputSource);
   ossimOpenCVSmoothFilter(ossimObject* owner,
                        ossimImageSource* inputSource);
   virtual ~ossimOpenCVSmoothFilter();
   ossimString getShortName()const
      {
         return ossimString("OpenCVSmooth");
      }
   
   ossimString getLongName()const
      {
         return ossimString("OpenCV Smooth Filter");
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
   int param1;
   int param2;
   double param3;
   double param4;

protected:
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   void runUcharTransformation(ossimImageData* tile);

TYPE_DATA
};

#endif
