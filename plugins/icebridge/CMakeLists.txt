cmake_minimum_required(VERSION 3.13)

if (STANDALONE)
    project(pdal-icebridge)
    include("${CMAKE_CURRENT_LIST_DIR}/../../cmake/standalone.cmake" )
    include(${PDAL_CMAKE_DIR}/arbiter.cmake)
    include(${PDAL_CMAKE_DIR}/nlohmann.cmake)
    include(${PDAL_CMAKE_DIR}/libxml2.cmake)
    add_subdirectory("${PROJECT_SOURCE_DIR}/../../vendor/arbiter" "vendor/arbiter")
    find_package(PDAL REQUIRED)
endif(STANDALONE)

#
# Icebridge plugin CMake configuration
#

include (${PDAL_CMAKE_DIR}/hdf5.cmake)


if (NOT PDAL_HAVE_HDF5)
    message(FATAL "HDF5 not found but is required for Icebridge.")
else()
    PDAL_ADD_PLUGIN(libname reader icebridge
        FILES
            io/IcebridgeReader.cpp
            io/Hdf5Handler.cpp
        LINK_WITH
            ${HDF5_LIBRARIES}
            ${PDAL_LIBRARIES}
        INCLUDES
            ${ROOT_DIR}
            ${LIBXML2_INCLUDE_DIR}
    )

    if (WITH_TESTS)
        PDAL_ADD_TEST(pdal_io_icebridge_reader_test
            FILES test/IcebridgeReaderTest.cpp
            LINK_WITH ${libname})
    endif()
endif()
