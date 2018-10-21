
#############################################################
# QT
#############################################################
set( CMAKE_PREFIX_PATH "C:/Qt/5.9.2/msvc2017_64" )
find_package(Qt5Core REQUIRED)
include_directories(${Qt5Core_INCLUDE_DIRS})
find_package(Qt5Widgets REQUIRED)
include_directories(${Qt5Widgets_INCLUDE_DIRS})

get_filename_component(QT_DIR ${QT_QMAKE_EXECUTABLE} DIRECTORY)
set( QT_INSTALL_FILES
    ${QT_DIR}/Qt5Core.dll
    ${QT_DIR}/Qt5Widgets.dll
    ${QT_DIR}/Qt5Gui.dll
)
file( COPY ${QT_INSTALL_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
install( FILES ${QT_INSTALL_FILES} DESTINATION . )
install( FILES ${QT_DIR}/../plugins/platforms/qwindows.dll DESTINATION ./platforms )

#############################################################
# BOOST
#############################################################
set( BOOST_DIR "${PROJECT_SOURCE_DIR}/../3rdparty/boost-vc141-1.64.0" )
include_directories( ${BOOST_DIR}/include )
link_directories( "${BOOST_DIR}/lib" )
set( BOOST_INSTALL_FILES
    ${BOOST_DIR}/bin/boost_zlib-vc141-mt-1_64.dll
    ${BOOST_DIR}/bin/boost_iostreams-vc141-mt-1_64.dll
    )
file( COPY ${BOOST_INSTALL_FILES} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} )
install( FILES ${BOOST_INSTALL_FILES} DESTINATION . )

#############################################################
# GTest
#############################################################
set( GTEST_DIR "${PROJECT_SOURCE_DIR}/../3rdparty/gtest-1.7.0" )
include_directories( ${GTEST_DIR}/include )
link_directories( "${GTEST_DIR}/lib" )
set( GTEST_LIBS
    optimized gtest
    optimized gtest_main
    )
