PROJECT (opencv-ossim-plugin)
   cmake_minimum_required(VERSION 2.6)

#####
# OpenCV
#####

FIND_PATH(CV_INCLUDE_DIR cv.h
      /usr/include/opencv
      /usr/local/include/opencv/cv/include
      $ENV{ProgramFiles}/OpenCV/cv/include
      $ENV{SystemDrive}/OpenCV/cv/include
      )

FIND_PATH(CXCORE_INCLUDE_DIR cvaux.h
      /usr/include/opencv
      /usr/local/include/opencv/cvaux/include
      $ENV{ProgramFiles}/OpenCV/cvaux/include
      $ENV{SystemDrive}/OpenCV/cvaux/include
      )

FIND_PATH(CVAUX_INCLUDE_DIR cxcore.h
      /usr/include/opencv
      /usr/local/include/opencv/cxcore/include
      $ENV{ProgramFiles}/OpenCV/cxcore/include
      $ENV{SystemDrive}/OpenCV/cxcore/include
      )

FIND_PATH(HIGHGUI_INCLUDE_DIR highgui.h
      /usr/include/opencv
      /usr/local/include/opencv/otherlibs/highgui
      $ENV{ProgramFiles}/OpenCV/otherlibs/highgui
      $ENV{SystemDrive}/OpenCV/otherlibs/highgui
      )

FIND_LIBRARY(CV_LIB NAMES cv PATHS
     /usr/lib
     /usr/local/lib
     /usr/lib/postgresql
     /usr/lib64
     /usr/local/lib64
     $ENV{ProgramFiles}/OpenCV/lib
     $ENV{SystemDrive}/OpenCV/lib
     )

FIND_LIBRARY(CVAUX_LIB NAMES cvaux PATHS
     /usr/lib
     /usr/local/lib
     /usr/lib/postgresql
     /usr/lib64
     /usr/local/lib64
     $ENV{ProgramFiles}/OpenCV/lib
     $ENV{SystemDrive}/OpenCV/lib
     )

FIND_LIBRARY(CXCORE_LIB NAMES cxcore PATHS
     /usr/lib
     /usr/local/lib
     /usr/lib/postgresql
     /usr/lib64
     /usr/local/lib64
     $ENV{ProgramFiles}/OpenCV/lib
     $ENV{SystemDrive}/OpenCV/lib
     )

FIND_LIBRARY(HIGHGUI_LIB NAMES highgui PATHS
     /usr/lib
     /usr/local/lib
     /usr/lib/postgresql
     /usr/lib64
     /usr/local/lib64
     $ENV{ProgramFiles}/OpenCV/lib
     $ENV{SystemDrive}/OpenCV/lib
     )

###
# OSSIM
###

FIND_PATH(OSSIM_INCLUDE_DIR ossim/ossimVersion.h
    /usr/include/ossim
    /c/msys/1.0/include
    c:/msys/1.0/include
    )

FIND_LIBRARY(OSSIM_LIB NAMES ossim PATHS
    /usr/lib
    /c/msys/1.0/lib
    c:/msys/1.0/lib
    )

###
# Create OpenCV Library Plugin for OSSIM
###

INCLUDE_DIRECTORIES(
    ${HIGHGUI_INCLUDE_DIR}
    ${OPENCV_INCLUDE_DIR}
    ${CXCORE_INCLUDE_DIR}
    ${CVAUX_INCLUDE_DIR}
    ${OSSIM_INCLUDE_DIR}
)

set(CMAKE_CXX_FLAGS "-g '-DCOMPLEX2=std::complex<double>'")
set(CMAKE_C_FLAGS "-g")

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

ADD_CUSTOM_COMMAND(OUTPUT ossimRegPluginLinux
	COMMAND g++ -shared -o libossimopencv_plugin.so ./CMakeFiles/ossimopencv.dir/*.o
	    ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
            -I${HIGHGUI_INCLUDE_DIR}
            -I${OPENCV_INCLUDE_DIR}
            -I${CXCORE_INCLUDE_DIR}
            -I${CVAUX_INCLUDE_DIR}
            -I${OSSIM_INCLUDE_DIR}
            DEPENDS ossimopencv
	)

ADD_CUSTOM_TARGET(ossim DEPENDS ossimRegPluginLinux)


###
# Build test applications 
###

# Smooth Filter
ADD_EXECUTABLE(test_smooth apps/smooth.cpp)
add_dependencies(test_smooth ossim)
TARGET_LINK_LIBRARIES(test_smooth
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)

# Canny Filter
ADD_EXECUTABLE(test_canny apps/canny.cpp)
add_dependencies(test_canny ossim)
TARGET_LINK_LIBRARIES(test_canny
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)

# Erode Filter
ADD_EXECUTABLE(test_erode apps/erode.cpp)
add_dependencies(test_erode ossim)
TARGET_LINK_LIBRARIES(test_erode
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)

# Dilate Filter
ADD_EXECUTABLE(test_dilate apps/dilate.cpp)
add_dependencies(test_dilate ossim)
TARGET_LINK_LIBRARIES(test_dilate
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)

# Sobel Filter
ADD_EXECUTABLE(test_sobel apps/sobel.cpp)
add_dependencies(test_sobel ossim)
TARGET_LINK_LIBRARIES(test_sobel
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)

# Laplace Filter
ADD_EXECUTABLE(test_laplace apps/laplace.cpp)
add_dependencies(test_laplace ossim)
TARGET_LINK_LIBRARIES(test_laplace
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)

# Threshold Filter
ADD_EXECUTABLE(test_threshold apps/threshold.cpp)
add_dependencies(test_threshold ossim)
TARGET_LINK_LIBRARIES(test_threshold
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)
# Pyramid Segmentation
ADD_EXECUTABLE(test_pyrsegmentation apps/pyrsegmentation.cpp)
add_dependencies(test_pyrsegmentation ossim)
TARGET_LINK_LIBRARIES(test_pyrsegmentation
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)

# Feature Detection 
ADD_EXECUTABLE(test_corners apps/corners.cpp)
add_dependencies(test_corners ossim)
TARGET_LINK_LIBRARIES(test_corners
            ossimopencv
            ${HIGHGUI_LIB}
            ${OPENCV_LIB}
            ${CXCORE_LIB}
            ${CVAUX_LIB}
            ${OSSIM_LIB}
)
