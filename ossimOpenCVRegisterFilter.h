#ifndef ossimOpenCVRegisterFilter_HEADER
#define ossimOpenCVRegisterFilter_HEADER
#include "ossim/plugin/ossimSharedObjectBridge.h"
#include "ossim/base/ossimString.h"


#include <ossim/imaging/ossimImageCombiner.h>
#include <ossim/imaging/ossimImageDataFactory.h>
#include <ossim/imaging/ossimAnnotationObject.h>
#include <ossim/base/ossimDpt.h>
#include <ossim/base/ossimGpt.h>
#include <ossim/projection/ossimImageViewProjectionTransform.h>

#include "cv.h"
#include "highgui.h"
class ossimOpenCVRegisterFilter : public ossimImageCombiner //ossimImageSourceFilter
{
public:
   ossimOpenCVRegisterFilter(ossimObject* owner=NULL);
 /*  ossimOpenCVRegisterFilter(ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);
   ossimOpenCVRegisterFilter(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double c1 = 1.0/3.0,
                        double c2 = 1.0/3.0,
                        double c3 = 1.0/3.0);*/
   virtual ~ossimOpenCVRegisterFilter();
   ossimString getShortName()const
      {
         return ossimString("OpenCVRegister");
      }
   
   ossimString getLongName()const
      {
         return ossimString("OpenCV Registration Filter");
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
   std::vector< ossimAnnotationObject * > 	theAnnotationObjectList;
   
	ossimImageViewProjectionTransform* transform;
	ossimImageViewProjectionTransform* transform2;
protected:
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   void runUcharTransformation(ossimImageData* left,ossimImageData* right);
   double theC1;
   double theC2;
   double theC3;
   CvMemStorage* storage ;
   int thresh ;
   int match(IplImage *vis, IplImage *ir,IplImage *irdeformada);


TYPE_DATA
};

#endif
