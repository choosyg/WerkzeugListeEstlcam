set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)

set( UIS
    MainWindow.ui
)
set_source_files_properties(${UIS} PROPERTIES HEADER_FILE_ONLY TRUE)

add_library( toolsUiLib
    MainWindow.h
    MainWindow.cpp
    ToolModel.h
    ToolModel.cpp
    ${UIS}
    )
target_compile_definitions( toolsUiLib PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)

add_executable( tools main.cpp )
target_link_libraries( tools
    estlcamLib
    toolsUiLib
    Qt5::Widgets
    Qt5::Core
    Qt5::Gui
)
set_target_properties(tools PROPERTIES WIN32_EXECUTABLE YES LINK_FLAGS "/ENTRY:mainCRTStartup")
target_compile_definitions( tools PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)
install(TARGETS tools RUNTIME DESTINATION .)

#add_executable( toolsTest
#    )
#target_link_libraries( toolsTest
#    toolsUiLib
#    Qt5::Widgets
#    Qt5::Core
#    Qt5::Gui
#    ${GTEST_LIBS}
#)
#target_compile_definitions( toolsTest PRIVATE BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)
#add_test(NAME toolsTest COMMAND ${CMAKE_BINARY_DIR}/app/toolsTest )


