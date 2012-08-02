cmake_minimum_required(VERSION 2.4.6)
include(/opt/ros/fuerte/share/ros/core/rosbuild/rosbuild.cmake)
#set(VTK_DIR /usr/local/lib/vtk-5.8)
#include(/usr/local/lib/vtk-5.8/VTKConfig.cmake)
#PROJECT(beginner_tutorials)
include_directories(/usr/local/include/vtk-5.8)

# Set the build type.  Options are:
#  Coverage       : w/ debug symbols, w/o optimization, w/ code-coverage
#  Debug          : w/ debug symbols, w/o optimization
#  Release        : w/o debug symbols, w/ optimization
#  RelWithDebInfo : w/ debug symbols, w/ optimization
#  MinSizeRel     : w/o debug symbols, w/ optimization, stripped binaries
#set(ROS_BUILD_TYPE RelWithDebInfo)

rosbuild_init()

FIND_PACKAGE(Qt4 REQUIRED)
set( QT_USE_QTGUI TRUE )

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

SET(beginner_tutorials_HEADERS src/dialog/MainWindow.h src/threads/rosThread.h src/dialog/pcldialog.h)

SET(beginner_tutorials_NHEADERS  src/pcl/pcprocessing.h) #src/pclViwer.h

SET(beginner_tutorials_SOURCES src/dialog/MainWindow.cpp src/threads/rosThread.cpp src/dialog/pcldialog.cpp src/pcl/pcprocessing.cpp) #src/pclViwer.cpp src/pcprocessing.cpp)

SET(beginner_tutorials_FORMS src/dialog/MainWindow.ui src/dialog/pcldialog.ui)

QT4_WRAP_CPP(beginner_tutorials_HEADERS_MOC ${beginner_tutorials_HEADERS})
QT4_WRAP_UI(uis_h ${beginner_tutorials_FORMS})

#ADD_DEFINITIONS(-DQT_NO_KEYWORDS)

qt4_automoc(${beginner_tutorials_SOURCES})

#set the default path for built executables to the "bin" directory
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
#set the default path for built libraries to the "lib" directory
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(/opt/ros/fuerte/stacks)
include_directories(./src/dialog)
include_directories(./src/pcl)
include_directories(./src/threads)

#uncomment if you have defined messages
#rosbuild_genmsg()
#uncomment if you have defined services
#rosbuild_gensrv()

#common commands for building c++ executables and libraries
rosbuild_add_library(${PROJECT_NAME} src/example.cpp)
FIND_PACKAGE( OpenCV REQUIRED )
FIND_PACKAGE(VTK REQUIRED)
INCLUDE(${VTK_USE_FILE})
target_link_libraries(${PROJECT_NAME} opencv_highgui opencv_imgproc opencv_core ${QT_LIBRARIES} QVTK)
#target_link_libraries(${PROJECT_NAME} ${QT_LIBRARIES}) -L/usr/local/lib/vtk-5.8 QVTK MapReduceMPI mpistubs vtkalglib vtkCommon vtkDICOMParser vtkexoIIc vtkexpat vtkCharts vtkQtChart vtkFiltering vtkfreetype vtkftgl vtkGenericFiltering vtkGeovis vtkGraphics vtkhdf5 vtkHybrid vtkImaging vtkInfovis vtkIO vtkjpeg vtklibxml2 vtkmetaio vtkNetCDF vtkNetCDF_cxx vtkpng vtkproj4  vtkRendering vtksqlite vtksys vtktiff vtkverdict vtkViews vtkVolumeRendering vtkWidgets vtkzlib
rosbuild_add_boost_directories()
rosbuild_link_boost(${PROJECT_NAME} thread)
rosbuild_add_executable(example src/example.cpp ${beginner_tutorials_SOURCES} ${beginner_tutorials_HEADERS_MOC} ${beginner_tutorials_NHEADERS} ${uis_h})
target_link_libraries(example ${PROJECT_NAME})
