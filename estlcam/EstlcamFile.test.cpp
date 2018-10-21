#include <gtest/gtest.h>
#include "EstlcamFile.h"

TEST( EstlcamFileTest, read ) {
    std::string rd = TEST_RESOURCES;
    EstlcamFile file( rd + "/test.tl" );
    auto list = file.read();

    for( const auto& tool : list ) {
        std::cout << tool.toString() << std::endl;
    }
}
