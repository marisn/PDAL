#------------------------------------------------------------------------------
# CPACK controls
#------------------------------------------------------------------------------

SET(CPACK_PACKAGE_VERSION_MAJOR ${PDAL_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${PDAL_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${PDAL_VERSION_MINOR})
SET(CPACK_PACKAGE_NAME "PDAL")

SET(CPACK_SOURCE_GENERATOR "TBZ2;TGZ")
SET(CPACK_PACKAGE_VENDOR "PDAL Development Team")
SET(CPACK_RESOURCE_FILE_LICENSE    "${PROJECT_SOURCE_DIR}/LICENSE.txt")

set(CPACK_SOURCE_PACKAGE_FILE_NAME
    "${CMAKE_PROJECT_NAME}-${PDAL_VERSION}-src")

set(CPACK_SOURCE_IGNORE_FILES
"/\\\\.gitattributes;/\\\\.vagrant;/\\\\.DS_Store;/CVS/;/\\\\.git/;\\\\.swp$;~$;\\\\.\\\\#;/\\\\#")

list(APPEND CPACK_SOURCE_IGNORE_FILES "CMakeScripts/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CMakeCache.txt")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".xcodeproj")
list(APPEND CPACK_SOURCE_IGNORE_FILES "build.make")
list(APPEND CPACK_SOURCE_IGNORE_FILES "_CPack_Packages")
list(APPEND CPACK_SOURCE_IGNORE_FILES "cmake_install.cmake")
list(APPEND CPACK_SOURCE_IGNORE_FILES "Testing")
list(APPEND CPACK_SOURCE_IGNORE_FILES "PDAL.build/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/bin/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/build/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "Makefile")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CMakeFiles")
list(APPEND CPACK_SOURCE_IGNORE_FILES "CTestTestfile.cmake")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/test/data/local/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/test/unit/TestConfig.hpp$")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/doc/doxygen/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/doc/build/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/doc/presentations/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/doc/_static/logo/dongle/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "/cmake/examples/")
list(APPEND CPACK_SOURCE_IGNORE_FILES "pdal_features.hpp$")
list(APPEND CPACK_SOURCE_IGNORE_FILES "package.sh")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".gz2")
list(APPEND CPACK_SOURCE_IGNORE_FILES ".bz2")

include(CPack)
