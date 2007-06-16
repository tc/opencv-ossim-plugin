#ifndef ossimOpenCVGoodFeaturesToTrack_HEADER
#define ossimOpenCVGoodFeaturesToTrack_HEADER
#include "ossim/plugin/ossimSharedObjectBridge.h"
#include "ossim/base/ossimString.h"


#include "ossim/imaging/ossimImageSourceFilter.h"
#include <ossim/imaging/ossimAnnotationObject.h>
#include "cv.h"

class ossimOpenCVGoodFeaturesToTrack : public ossimImageSourceFilter //ossimImageSourceFilter
{
public:
   ossimOpenCVGoodFeaturesToTrack(ossimObject* owner=NULL);
   ossimOpenCVGoodFeaturesToTrack(ossimImageSource* inputSource,
                        double c1 = 0.1,
                        double c2 = 10,
                        double c3 = 1.0/3.0);
   ossimOpenCVGoodFeaturesToTrack(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double c1 = 0.1,
                        double c2 = 10,
                        double c3 = 1.0/3.0);
   virtual ~ossimOpenCVGoodFeaturesToTrack();
   ossimString getShortName()const
      {
         return ossimString("OpenCV GoodFeaturesToTrack");
      }
   
   ossimString getLongName()const
      {
         return ossimString("OpenCV GoodFeatures Identifier");
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

    std::vector<ossimDpt>      thePoints;
protected:
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   void runUcharTransformation(ossimImageData* tile);
   double quality_level;
   double min_distance;
   double theC3;
   CvMemStorage* storage ;
   int thresh ;
   
TYPE_DATA
};

#endif
