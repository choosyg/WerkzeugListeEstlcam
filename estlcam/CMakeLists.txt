add_library( estlcamLib
    EstlcamFile.h    
    EstlcamFile.cpp
    )
target_compile_definitions(estlcamLib PUBLIC BOOST_ALL_DYN_LINK _WIN32_WINNT=0x0501)

add_executable( estlcamLibTest
    EstlcamFile.test.cpp
    )
target_link_libraries( estlcamLibTest
    estlcamLib
    ${GTEST_LIBS}
)
target_compile_definitions( estlcamLibTest PRIVATE
    TEST_RESOURCES="${CMAKE_SOURCE_DIR}/testdata"
    )
add_test(NAME estlcamLibTest COMMAND ${CMAKE_BINARY_DIR}/estlcamLib/estlcamLibTest )
