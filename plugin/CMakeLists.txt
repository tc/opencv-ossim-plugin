###
# Create OpenCV Library Plugin for OSSIM
###

ADD_LIBRARY(ossimopencv
	ossimOpenCVCannyFilter.cpp
 	ossimOpenCVDilateFilter.cpp
	ossimOpenCVErodeFilter.cpp
	ossimOpenCVSmoothFilter.cpp
	ossimOpenCVLaplaceFilter.cpp
	ossimOpenCVSobelFilter.cpp
	ossimOpenCVThresholdFilter.cpp
	ossimOpenCVGoodFeaturesToTrack.cpp
	ossimOpenCVPyrSegmentation.cpp
	ossimOpenCVPluginFactory.cpp
	ossimOpenCVPluginInit.cpp
)

IF(UNIX)
	ADD_CUSTOM_COMMAND(OUTPUT ossimRegPluginLinux
		COMMAND g++ -shared -o libossimopencv_plugin.so ./CMakeFiles/ossimopencv.dir/*.o
			${HIGHGUI_LIB}
				${CV_LIB}
				${CXCORE_LIB}
				${CVAUX_LIB}
				${OSSIM_LIB}
				-I${HIGHGUI_INCLUDE_DIR}
				-I${CV_INCLUDE_DIR}
				-I${CXCORE_INCLUDE_DIR}
				-I${CVAUX_INCLUDE_DIR}
				-I${OSSIM_INCLUDE_DIR}
				DEPENDS ossimopencv
		)

	ADD_CUSTOM_TARGET(ossim DEPENDS ossimRegPluginLinux)
ENDIF(UNIX)

