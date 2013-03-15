#include <sstream>
#include <iostream>

#include <gtest/gtest.h>

#include <utils/log.hh>
#include "../constant.hh"
#include "../map.hh"
#include "../cell.hh"
#include "../game.hh"

#include "../action-move.hh"

class ActionsTest : public ::testing::Test
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


            map_ = new Map();
            if (map_->load(f))
                FAIL() << "Map::load() failed";

            rules::Players_sptr players(
                new rules::Players {
                    std::vector<rules::Player_sptr>
                    {
                        rules::Player_sptr(new rules::Player(0, 0)),
                        rules::Player_sptr(new rules::Player(1, 0)),
                    }
                }
            );

            gamestate_ = new GameState(map_, players);
            gamestate_->init();

            gamestate_->add_boat({3, 3}, 0, BATEAU_GALION); // 0
            gamestate_->get_boat(0)->deplacable = true;

            gamestate_->add_boat({3, 6}, 1, BATEAU_GALION); // 1
            gamestate_->get_boat(1)->deplacable = true;

            gamestate_->add_boat({5, 8}, 0, BATEAU_GALION); // 2
            gamestate_->get_boat(2)->deplacable = false;

            gamestate_->add_boat({31, 31}, 0, BATEAU_GALION); // 3
            gamestate_->get_boat(3)->deplacable = true;

            gamestate_->add_boat({3, 3}, 0, BATEAU_CARAVELLE); // 4
            gamestate_->get_boat(4)->deplacable = true;

        }

        std::stringstream f;
        Map* map_;
        GameState* gamestate_;
};

TEST_F(ActionsTest, MoveCheckTest)
{
    ActionMove a1(672324, { 3, 4 }, 0);
    EXPECT_EQ(ID_INVALIDE, a1.check(gamestate_)) << "ID should be invalid";

    ActionMove a2(1, { 3, 4 }, 0);
    EXPECT_EQ(BATEAU_ENNEMI, a2.check(gamestate_)) << "Boat should be enemy";

    ActionMove a3(2, { 3, 4 }, 0);
    EXPECT_EQ(NON_DEPLACABLE, a3.check(gamestate_))
        << "Boat shouldn't be movable";

    ActionMove a6(3, { TAILLE_TERRAIN, 31 }, 0);
    EXPECT_EQ(POSITION_INVALIDE, a6.check(gamestate_))
        << "Destination position should be invalid";

    ActionMove a7(0, {3 + GALION_DEPLACEMENT, 3}, 0);
    EXPECT_EQ(OK, a7.check(gamestate_))
        << "Should be OK";

    ActionMove a8(0, {3, 3 + GALION_DEPLACEMENT}, 0);
    EXPECT_EQ(OK, a8.check(gamestate_))
        << "Should be OK";

    ActionMove a9(0, {3 + 2, 3 + 2}, 0);
    EXPECT_EQ(OK, a9.check(gamestate_))
        << "Should be OK";

    ActionMove a10(0, {3 + GALION_DEPLACEMENT, 3 + GALION_DEPLACEMENT}, 0);
    EXPECT_EQ(TROP_LOIN, a10.check(gamestate_))
        << "Destination should be too far";

    ActionMove a11(0, {3 + GALION_DEPLACEMENT + 1, 3}, 0);
    EXPECT_EQ(TROP_LOIN, a11.check(gamestate_))
        << "Destination should be too far";

    ActionMove a12(4, {3 + CARAVELLE_DEPLACEMENT, 3}, 0);
    EXPECT_EQ(OK, a7.check(gamestate_))
        << "Should be OK";

    ActionMove a13(4, {3, 3 + CARAVELLE_DEPLACEMENT}, 0);
    EXPECT_EQ(OK, a8.check(gamestate_))
        << "Should be OK";

    ActionMove a14(4, {3 + 2, 3 + 2}, 0);
    EXPECT_EQ(OK, a9.check(gamestate_))
        << "Should be OK";

    ActionMove a15(4, {3+CARAVELLE_DEPLACEMENT, 3+CARAVELLE_DEPLACEMENT}, 0);
    EXPECT_EQ(TROP_LOIN, a10.check(gamestate_))
        << "Destination should be too far";

    ActionMove a16(4, {3 + CARAVELLE_DEPLACEMENT + 1, 3}, 0);
    EXPECT_EQ(TROP_LOIN, a11.check(gamestate_))
        << "Destination should be too far";
}

TEST_F(ActionsTest, MoveTest)
{
    Cell* c;

    c = gamestate_->get_map()->get_cell({3, 3});
    EXPECT_TRUE(c->exists_boat(0)) << "Not in the original position";

    ActionMove a1(0, {4, 3}, 0); // Origin : 3, 3
    a1.apply_on(gamestate_);

    std::map<int, bateau> boats = gamestate_->get_boats();
    EXPECT_EQ(1, boats.count(0)) << "Boat has disappeared from boat std::map";

    c = gamestate_->get_map()->get_cell({3, 3});
    EXPECT_FALSE(c->exists_boat(0)) << "Still in the original position";

    c = gamestate_->get_map()->get_cell({4, 3});
    EXPECT_TRUE(c->exists_boat(0)) << "Not in the destination position";

    bateau boat = gamestate_->get_boats()[0];
    EXPECT_EQ(boat.pos, (position {4, 3})) << "bateau.pos hasn't been updated";
}
