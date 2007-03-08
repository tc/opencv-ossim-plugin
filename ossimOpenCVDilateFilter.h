#ifndef ossimOpenCVDilateFilter_HEADER
#define ossimOpenCVDilateFilter_HEADER
#include "ossim/plugin/ossimSharedObjectBridge.h"
#include "ossim/base/ossimString.h"


#include "ossim/imaging/ossimImageSourceFilter.h"

#include "cv.h"

class ossimOpenCVDilateFilter : public ossimImageSourceFilter
{
public:
   ossimOpenCVDilateFilter(ossimObject* owner=NULL);
   ossimOpenCVDilateFilter(ossimImageSource* inputSource);
   ossimOpenCVDilateFilter(ossimObject* owner,
                        ossimImageSource* inputSource);
   virtual ~ossimOpenCVDilateFilter();
   ossimString getShortName()const
      {
         return ossimString("Eroe");
      }
   
   ossimString getLongName()const
      {
         return ossimString("ossimOpenCVDilateFilter");
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

TYPE_DATA
};

#endif
