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
                        double t1 = 0.0,
                        double t2 = 0.0,
                        double t3 = 0.0,
                        int type = 0);
   ossimOpenCVThresholdFilter(ossimObject* owner,
                        ossimImageSource* inputSource,
                        double t1 = 0.0,
                        double t2 = 0.0,
                        double t3 = 0.0,
                        int type = 0);
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

   /*
   * Methods to expose thresholds for adjustment through the GUI
   */
   virtual void setProperty(ossimRefPtr<ossimProperty> property);
   virtual ossimRefPtr<ossimProperty> getProperty(const ossimString& name)const;
   virtual void getPropertyNames(std::vector<ossimString>& propertyNames)const;

protected:
   ossimRefPtr<ossimImageData> theBlankTile;
   ossimRefPtr<ossimImageData> theTile;
   void runUcharTransformation(ossimImageData* tile);
   double theThresh1;
   double theThresh2;
   double theThresh3;
/** Types of thresholding  <br>
CV_THRESH_BINARY      0   value = value > threshold ? max_value : 0       <br>
CV_THRESH_BINARY_INV  1  value = value > threshold ? 0 : max_value       <br>
CV_THRESH_TRUNC       2  value = value > threshold ? threshold : value   <br>
CV_THRESH_TOZERO      3   value = value > threshold ? value : 0           <br>
CV_THRESH_TOZERO_INV  4  value = value > threshold ? 0 : value           <br>
CV_THRESH_MASK        7<br>
CV_THRESH_OTSU        8  use Otsu algorithm to choose the optimal threshold value;<br>
                                    combine the flag with one of the above CV_THRESH_* values <br>
**/
   int theType;

TYPE_DATA
};

#endif
