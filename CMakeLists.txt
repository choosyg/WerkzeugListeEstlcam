cmake_minimum_required (VERSION 3.9.0)
project( EstlcamTools )

set( EstlcamTools_VERSION_MAJOR 1 )
set( EstlcamTools_VERSION_MINOR 0 )
set( EstlcamTools_VERSION_PATCH 0 )

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include( scripts/thirdparty.cmake )
include(CTest)
enable_testing()

include_directories("${EstlcamTools_SOURCE_DIR}")
add_subdirectory( estlcam )
add_subdirectory( app )

set(CPACK_GENERATOR "ZIP")
set(CPACK_PACKAGE_VENDOR "Schiffler")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "It's great")
set(CPACK_PACKAGE_VERSION ${EstlcamTools_VERSION_MAJOR}.${EstlcamTools_VERSION_MINOR}.${EstlcamTools_VERSION_PATCH}-${CMAKE_BUILD_TYPE})
set(CPACK_PACKAGE_VERSION_MAJOR ${EstlcamTools_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${EstlcamTools_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${EstlcamTools_VERSION_PATCH})
include(CPack)
