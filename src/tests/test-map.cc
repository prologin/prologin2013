#include <sstream>
#include <iostream>

#include <gtest/gtest.h>

#include <utils/log.hh>
#include "../constant.hh"
#include "../map.hh"
#include "../cell.hh"

class MapTest : public ::testing::Test
{
    protected:
    virtual void SetUp()
    {
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;

        f << "2 0\n";
        f << "7 7\n";
        f << "~~o~~~~~~~~~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~~~~~~~~~~~~~o~~~~~~~\n";
        f << "~~o~~~~~~~~~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~o~~~~~~~~~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~^~~~~~~~~~~~o~~~~~~~\n";
        f << "~~~~~~~~~~~~^~~~~~~~~~~~o~~~~~~~\n";
        f << "~~o~~~~o~~~~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~o~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~^~~~~~~~~~~~^~~~~~~~~o~~~~~~~\n";
        f << "~~~~~~~~~~o~~~~~~~~~~~~~~~~o~~~~\n";
        f << "~~~o~~~~~~~~~o~~~~~~~o~~~~~~~~~~\n";
        f << "~^~~~~~~~~o~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~~~~~~o~~~~~~~~~o~~~~\n";
        f << "~~~~~~~~~~~~~~~~~o~~~~~~~~~o~~~~\n";
        f << "~~~~~~~~~~~~^~~~~~~~~~~~o~~~~~~~\n";
        f << "~~o~~~~o~~~~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~o~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~^~~~~~~~~~~~^~~~~~~~~o~~~~~~~\n";
        f << "~~~~~~~~~~o~~~~~~~~~~~~~~~~o~~~~\n";
        f << "~~~o~~~~~~~~~o~~~~~~~o~~~~~~~~~~\n";
        f << "~^~~~~~~~~o~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~~~~~~o~~~~~~~~~o~~~~\n";
        f << "~~~~~~~~~~~~~~~~~o~~~~~~~~~o~~~~\n";
        f << "~~o~~~~o~~~~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~o~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~^~~~~~~~~~~~^~~~~~~~~o~~~~~~~\n";
        f << "~~~~~~~~~~o~~~~~~~~~~~~~~~~o~~~~\n";
        f << "~~~o~~~~~~~~~o~~~~~~~o~~~~~~~~~~\n";
        f << "~^~~~~~~~~o~~~~~~~~~~~~~~~o~~~~~\n";
        f << "~~~~~~~~~~~~~~~~~o~~~~~~~~~~~~~~\n";
        f << "~~~~~~~~~~~~~~~~~o~~~~~~~~~o~~~~\n";
    }

    std::stringstream f;
};

TEST_F(MapTest, MapCreateFromFile)
{

    Map map;
    int err;
    if ((err = map.load(f)) != 0)
        FAIL() << "Map::load() returned " << err;

    INFO("get_cell (0, 0)");
    EXPECT_EQ(TERRAIN_MER, map.get_cell(position {0, 0})->get_type())
    << "Cell (0, 0) has a bad type";

    INFO("get_cell (2, 2)");
    EXPECT_EQ(TERRAIN_ILE, map.get_cell(position {2, 2})->get_type())
    << "Cell (2, 2) has a bad type";

    INFO("get_cell (1, 12)");
    EXPECT_EQ(TERRAIN_VOLCAN, map.get_cell(position {1, 12})->get_type())
    << "Cell (1, 12) has a bad type";

    INFO("get_cell (3, 3)");
    EXPECT_EQ(TERRAIN_MER, map.get_cell(position {3, 3})->get_type())
    << "Cell (3, 3) has a bad type";

    INFO("get_cell (31, 27)");
    EXPECT_EQ(TERRAIN_ILE, map.get_cell(position {27, 31})->get_type())
    << "Cell (31, 27) has a bad type";

    INFO("get_cell (32, 5)");
    EXPECT_EQ(NULL, map.get_cell(position {32, 5}))
    << "Cell (32, 5) has a bad type";
}
