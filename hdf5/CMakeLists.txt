# 
# alps-hdf5 cmake configuration script.
# 

cmake_minimum_required (VERSION 2.8)

# define project name
project (alps-hdf5 CXX)
message ("\nALPSCore module : ${PROJECT_NAME}\n")

# add common modules from ../common/cmake
list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/../common/cmake)

include(ALPSCommonModuleDefinitions)
include(ALPSEnableMPI)

# Boost
#set(Boost_USE_STATIC_LIBS        ON)
#set(Boost_USE_STATIC_RUNTIME    OFF)
find_package (Boost COMPONENTS filesystem system REQUIRED)
    message(STATUS "Boost includes: " ${Boost_INCLUDE_DIRS} )
    message(STATUS "Boost libs: " ${Boost_LIBRARIES} )
find_package (HDF5 REQUIRED)
    message(STATUS "HDF5 includes: " ${HDF5_INCLUDE_DIRS} )
    message(STATUS "HDF5 libs: " ${HDF5_LIBRARIES} )

if (NOT DEFINED ALPS_GLOBAL_BUILD)
find_package (alps-utility REQUIRED)
    message(STATUS "alps-utility includes: " ${alps-utility_INCLUDE_DIRS} )
    message(STATUS "alps-utility libs: " ${alps-utility_LIBRARIES} )
endif(NOT DEFINED ALPS_GLOBAL_BUILD)

# LINK_ALL - link for libraries
set (LINK_ALL ${Boost_LIBRARIES} ${HDF5_LIBRARIES} ${alps-utility_LIBRARIES})

include_directories(
    ${Boost_INCLUDE_DIRS}
    ${HDF5_INCLUDE_DIRS}
    ${alps-utility_INCLUDE_DIRS}
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_BINARY_DIR}/include
)

#configure_file("${PROJECT_SOURCE_DIR}/include/config.h.in" "${PROJECT_BINARY_DIR}/include/alps/config.h")
add_subdirectory(src)

# add tests - take two parameters Testing and TestXMLOutput
option(Testing "Enable testing" ON)
if (Testing)
    enable_testing()
    add_subdirectory(test)
endif (Testing)

# installation
install(DIRECTORY include DESTINATION .
          FILES_MATCHING PATTERN "*.hpp" PATTERN "*.hxx"
        )

# Documentation
set(DOXYFILE_SOURCE_DIR "./include")
include(UseDoxygen)

# Generate pkg-config file
configure_file("${PROJECT_SOURCE_DIR}/${PROJECT_NAME}.pc.in" "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc")
install(FILES "${PROJECT_BINARY_DIR}/${PROJECT_NAME}.pc" DESTINATION "lib/pkgconfig")

# Generate cmake Find module
set(PROJECT_SEARCH_FILE alps/hdf5.hpp)
configure_file("${PROJECT_SOURCE_DIR}/../common/cmake/FindALPSModule.cmake.in" "${PROJECT_BINARY_DIR}/Find${PROJECT_NAME}.cmake" @ONLY)
install(FILES "${PROJECT_BINARY_DIR}/Find${PROJECT_NAME}.cmake" DESTINATION "share/cmake/Modules")

