cmake_minimum_required(VERSION 2.6)
project(PWIZ_external_build)

###########################################################################
### System specific configuration 
###########################################################################

# add executables here...
SET(my_executables 
  hello_crossover 
)

set(PWIZ_BUILDNAME "build-linux-x86_64")
set(PWIZ_MAKEDIR "/gcc-4.6/release/link-static/runtime-link-static/threading-multi/")

# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #
# No Edits beyond this place
# # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # # #

###########################################################################
### 1. Configure proteowizard
###########################################################################

if (NOT PWIZ_MAINDIR)
  message(FATAL_ERROR "Aborting ..., please specify the pwiz build directory (-DPWIZ_MAINDIR)")
endif()

# Set the proteowizard library and base directory
set(PWIZ_LIBBASEDIR "${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/libraries/")
set(PWIZ_BASEDIR "${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/pwiz/")

#set all pwiz-specific libs that are needed
set(pwiz_link_libraries
  pwiz_data_msdata
  pwiz_data_msdata_version
  pwiz_utility_misc
  SHA1
  pwiz_utility_minimxml
  pwiz_data_common
  pwiz_data_msdata_mz5
)

#set all external libs that are needed
set(ext_link_libraries
  boost_date_time-gcc46-mt-s     # not strictly necessary
  boost_prg_exec_monitor-gcc46-mt-s     # not strictly necessary
  boost_program_options-gcc46-mt-s     # not strictly necessary
  boost_serialization-gcc46-mt-s     # not strictly necessary
  boost_thread-gcc46-mt-s
  boost_regex-gcc46-mt-s
  boost_system-gcc46-mt-s
  boost_filesystem-gcc46-mt-s
  boost_iostreams-gcc46-mt-s
  z-gcc46-mt-s-1_2
  hdf5pp
  hdf5
  pthread
)

include_directories(${PWIZ_MAINDIR})

# add the locations of the libraries of pwiz 
link_directories(${PWIZ_BASEDIR}/data/msdata/${PWIZ_MAKEDIR})
link_directories(${PWIZ_BASEDIR}/utility/misc/${PWIZ_MAKEDIR})
link_directories(${PWIZ_BASEDIR}/utility/minimxml/${PWIZ_MAKEDIR})
link_directories(${PWIZ_BASEDIR}/data/common/${PWIZ_MAKEDIR})
link_directories(${PWIZ_BASEDIR}/data/msdata/mz5/${PWIZ_MAKEDIR})

# add the locations of the external libraries used by proteowizard
link_directories(${PWIZ_LIBBASEDIR}/hdf5-1.8.7/${PWIZ_MAKEDIR})
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/date_time-1_43_0/boost/${PWIZ_MAKEDIR}) # not strictly necessary
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/prg_exec_monitor-1_43_0/boost/${PWIZ_MAKEDIR}) # not strictly necessary
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/serialization-1_43_0/boost/${PWIZ_MAKEDIR}) # not strictly necessary
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/program_options-1_43_0/boost/${PWIZ_MAKEDIR}) # not strictly necessary
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/filesystem-1_43_0/boost/${PWIZ_MAKEDIR})
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/iostreams-1_43_0/boost/${PWIZ_MAKEDIR})
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/thread-1_43_0/boost/${PWIZ_MAKEDIR})
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/regex-1_43_0/boost/${PWIZ_MAKEDIR})
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/boost/system-1_43_0/boost/${PWIZ_MAKEDIR})
link_directories(${PWIZ_MAINDIR}/${PWIZ_BUILDNAME}/ext/zlib/zlib/zlib/gcc-4.6/release/1.2.3/link-static/runtime-link-static/threading-multi/)

###########################################################################
### 2. Configure OpenMS
###########################################################################

find_package(OpenMS)
# check whether the OpenMS package was found
if (OpenMS_FOUND)
  message(STATUS "<open-ms/include>: ${OPENMS_INCLUDE_DIRS}")
  message(STATUS "<open-ms/lib>: ")
  message(STATUS "<open-ms/flags>: ${OPENMS_ADDCXX_FLAGS}")
  message(STATUS "<open-ms/definitions/>: ${OPENMS_DEFINITIONS}")
  ## include directories for OpenMS headers (and contrib)
  set(inc ${inc} ${OPENMS_INCLUDE_DIRS})
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OPENMS_ADDCXX_FLAGS}")
else(OpenMS_FOUND)
  message(FATAL_ERROR "OpenMSConfig.cmake file not found!")
endif(OpenMS_FOUND)

message(STATUS ">>>>>>Using the following variables:<<<<<<")
message(STATUS "src -------- ${src} -------")
message(STATUS "lib -------- ${lib} -------")
message(STATUS "inc -------- ${inc} -------")

include_directories(${inc})
link_directories(${lib})

# set the compile flags
set(CMAKE_CXX_FLAGS "-g -Wall ")

foreach(i ${my_executables})
  add_executable(${i} ${i}.cpp)
endforeach(i)

# Link the pwiz and the external libraries in
foreach(i ${pwiz_link_libraries})
  foreach(j ${my_executables})
    target_link_libraries(${j} ${i})
  endforeach(j)
endforeach(i)
foreach(i ${ext_link_libraries})
  foreach(j ${my_executables})
    target_link_libraries(${j} ${i})
  endforeach(j)
endforeach(i)

# Link the OpenMS library
foreach(j ${my_executables})
  target_link_libraries(${j} OpenMS)
endforeach(j)


