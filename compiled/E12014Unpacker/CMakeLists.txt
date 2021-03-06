# CMakeLists.txt for event package. It creates a library with dictionary and a main program
cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(StandAloneATTPCROOT)

SET(BOOST_ROOT "/mnt/data/fair_install/boost_1_67_0/")
SET(PCL_DIR "/mnt/data/fair_install/pcl_1.8.1/share/pcl-1.8/")

# Check for needed environment variables
IF(NOT DEFINED ENV{FAIRROOTPATH})
  MESSAGE(FATAL_ERROR "You did not define the environment variable FAIRROOTPATH which is needed to find FairRoot. Please set this variable and execute cmake again.")
ENDIF(NOT DEFINED ENV{FAIRROOTPATH})

IF(NOT DEFINED ENV{SIMPATH})
   MESSAGE(FATAL_ERROR "You did not define the environment variable SIMPATH which is nedded to find the external packages. Please set this variable and execute cmake again.")
ENDIF(NOT DEFINED ENV{SIMPATH})

SET(SIMPATH $ENV{SIMPATH})
SET(FAIRROOTPATH $ENV{FAIRROOTPATH})
SET(ATTPCROOTPATH $ENV{VMCWORKDIR})

# You need to tell CMake where to find the ROOT installation. This can be done in a number of ways:
#   - ROOT built with classic configure/make use the provided $ROOTSYS/etc/cmake/FindROOT.cmake
#   - ROOT built with CMake. Add in CMAKE_PREFIX_PATH the installation prefix for ROOT
list(APPEND CMAKE_PREFIX_PATH $ENV{ROOTSYS})

#---Locate the ROOT package and defines a number of variables (e.g. ROOT_INCLUDE_DIRS)
find_package(ROOT REQUIRED COMPONENTS RIO)

#---Define useful ROOT functions and macros (e.g. ROOT_GENERATE_DICTIONARY)
include(${ROOT_USE_FILE})

#---Locate other external packages
find_package(Boost 1.67 REQUIRED)
find_package(PCL 1.8 REQUIRED)
find_package(HDF5 REQUIRED)

include_directories( ${ROOT_INCLUDE_DIR}
                     ${Boost_INCLUDE_DIRS}
                     ${BASE_INCLUDE_DIRECTORIES}
                     ${ATTPCROOTPATH}/include
                     ${FAIRROOTPATH}/include
                     ${CMAKE_SOURCE_DIR}
                     ${PCL_INCLUDE_DIRS}
		     ${HDF5_INCLUDE_DIR}
		     )

link_directories( ${FAIRROOT_LIBRARY_DIR}
  ${ATTPCROOTPATH}/build/lib
  ${FAIRROOTPATH}/lib
  ${PCL_LIBRARY_DIRS}
  ${ROOT_LIBRARY_DIR}
  ${FAIRROOT_LIBRARY_DIR}
  )

add_definitions(${PCL_DEFINITIONS})

message(STATUS ${ATTPCROOTPATH})

add_library(SAMain SHARED SAMain.cc)
target_link_libraries(SAMain ${ROOT_LIBRARIES} ATTPCReco FairTools Base ${PCL_LIBRARIES})

add_executable(SAMainExe SAMain.cc)


target_link_libraries (SAMainExe SAMain
  Base
  ParBase
    XMLParser
# Add all library names your code depending on.
# Basically those are the libraries in the header files' directory.
    AtTpcMap
    ATPar
    Spectrum
    ${HDF5_LIBRARIES}
    ${Boost_LOG_LIBRARY}
    ${Boost_LOG_SETUP_LIBRARY}
    ${Boost_THREAD_LIBRARY}
    ${Boost_DATE_TIME_LIBRARY}
    ${Boost_FILESYSTEM_LIBRARY}
    ${Boost_SYSTEM_LIBRARY}
    ${PCL_LIBRARIES}
)
#  ${ROOT_LIBRARIES} ${PCL_LIBRARIES}
#  ATPar
#  ${HDF5_LIBRARIES}
#  Core
#  Gpad
#  Graf
#  Graf3d
#  Gui
#  Hist
#  MathCore
#  Matrix
#  Minuit
#  Net
#  Physics
#  Postscript
#  Rint
#  RIO
#  Thread
#  Tree
#  TreePlayer
#  ATTPCReco
#  AtTpcMap
#  FairTools
#  Base)
  
add_executable(Visualizer visualizer.cpp)
target_link_libraries (Visualizer
  ${ROOT_LIBRARIES}
  ParBase
  XMLParser
  # Add all library names your code depending on.
  # Basically those are the libraries in the header files' directory.
  ATTPCReco
  AtTpcMap
  ATPar
  Spectrum
  ${HDF5_LIBRARIES}
  ${Boost_LOG_LIBRARY}
  ${Boost_LOG_SETUP_LIBRARY}
  ${Boost_THREAD_LIBRARY}
  ${Boost_DATE_TIME_LIBRARY}
  ${Boost_FILESYSTEM_LIBRARY}
  ${Boost_SYSTEM_LIBRARY}
  ${PCL_LIBRARIES}
  Base
  )