#ifndef ossimOpenCVCannyFilter_HEADER
#define ossimOpenCVCannyFilter_HEADER
#include "ossim/plugin/ossimSharedObjectBridge.h"
#include "ossim/base/ossimString.h"


#include "ossim/imaging/ossimImageSourceFilter.h"

#include "cv.h"

class ossimOpenCVCannyFilter : public ossimImageSourceFilter
{
public:
   ossimOpenCVCannyFilter(ossimObject* owner=NULL);
   ossimOpenCVCannyFilter(ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        int c3 = 3);
   ossimOpenCVCannyFilter(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        int c3 = 3);
   virtual ~ossimOpenCVCannyFilter();
   ossimString getShortName()const
      {
         return ossimString("OpenCVCanny");
      }
   
   ossimString getLongName()const
      {
         return ossimString("OpenCV Canny Filter");
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
   int theC3;

TYPE_DATA
};

#endif
