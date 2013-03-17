#include <sstream>
#include <iostream>

#include <gtest/gtest.h>

#include <utils/log.hh>
#include "../constant.hh"
#include "../map.hh"
#include "../cell.hh"
#include "../game.hh"

#include "../action-move.hh"
#include "../action-colonize.hh"
#include "../action-construct.hh"
#include "../action-charge.hh"
#include "../action-discharge.hh"

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
            f << "~~~~^~~~~~~~~~~~~~~~~~~~~~o~~~~~\n";
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

            gamestate_->add_boat({2, 4}, 0, BATEAU_CARAVELLE); // 5
            gamestate_->get_boat(4)->deplacable = true;

            gamestate_->add_boat({2, 4}, 1, BATEAU_CARAVELLE); // 6
            gamestate_->get_boat(4)->deplacable = true;

            gamestate_->add_boat({2, 5}, 0, BATEAU_CARAVELLE); // 7
            gamestate_->get_boat(4)->deplacable = true;

        }

        std::stringstream f;
        Map* map_;
        GameState* gamestate_;
};

TEST_F(ActionsTest, ChargeCheckTest)
{
    ActionCharge a1(5, {-1, 0}, 1, 0);
    EXPECT_EQ(POSITION_INVALIDE, a1.check(gamestate_))
        << "Position should be invalid";

    ActionCharge a2(5, {1, TAILLE_TERRAIN}, 1, 0);
    EXPECT_EQ(POSITION_INVALIDE, a2.check(gamestate_))
        << "Position should be invalid";

    ActionCharge a3(5, {0, 0}, 1, 0);
    EXPECT_EQ(ILE_INVALIDE, a3.check(gamestate_))
        << "Island should be invalid";

    gamestate_->get_map()->get_cell({2, 4})->set_player(1);
    ActionCharge a4(5, {2, 4}, 1, 0);
    EXPECT_EQ(ILE_ENNEMIE, a4.check(gamestate_))
        << "Island should be enemy";

    gamestate_->get_map()->get_cell({2, 4})->set_player(0);
    gamestate_->get_map()->get_cell({2, 4})->set_gold(0);
    ActionCharge a5(5, {2, 4}, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a5.check(gamestate_))
        << "There shouldn't be enough gold";

    gamestate_->get_map()->get_cell({2, 4})->set_gold(4);
    ActionCharge a6(5, {2, 4}, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a6.check(gamestate_))
        << "There shouldn't be enough gold";

    gamestate_->get_map()->get_cell({2, 4})->set_gold(5);
    ActionCharge a7(6, {2, 4}, 1, 0);
    EXPECT_EQ(BATEAU_ENNEMI, a7.check(gamestate_))
        << "Boat should be enemy";

    gamestate_->get_map()->get_cell({2, 4})->set_gold(5);
    ActionCharge a8(7, {2, 4}, 1, 0);
    EXPECT_EQ(POSITION_INVALIDE, a8.check(gamestate_))
        << "Boat shuldn't be in the same cell than the island";

    gamestate_->get_map()->get_cell({2, 4})->set_gold(5);
    ActionCharge a9(5, {2, 4}, 5, 0);
    EXPECT_EQ(OK, a9.check(gamestate_))
        << "Should be OK";
}

TEST_F(ActionsTest, ChargeTest)
{
    Cell* c = gamestate_->get_map()->get_cell({2, 4});
    c->set_player(0);

    c->set_gold(5);
    ActionCharge a1(5, {2, 4}, 3, 0);
    a1.apply_on(gamestate_);
    EXPECT_EQ(2, c->get_gold())
        << "Only 2 gold should remain";
    EXPECT_EQ(3, gamestate_->get_boat(5)->nb_or)
        << "Only 3 gold should remain";
}


TEST_F(ActionsTest, DischargeCheckTest)
{
    ActionDischarge a1(5, {-1, 0}, 1, 0);
    EXPECT_EQ(POSITION_INVALIDE, a1.check(gamestate_))
        << "Position should be invalid";

    ActionDischarge a2(5, {1, TAILLE_TERRAIN}, 1, 0);
    EXPECT_EQ(POSITION_INVALIDE, a2.check(gamestate_))
        << "Position should be invalid";

    ActionDischarge a3(5, {0, 0}, 1, 0);
    EXPECT_EQ(ILE_INVALIDE, a3.check(gamestate_))
        << "Island should be invalid";

    gamestate_->get_map()->get_cell({2, 4})->set_player(1);
    ActionDischarge a4(5, {2, 4}, 1, 0);
    EXPECT_EQ(ILE_ENNEMIE, a4.check(gamestate_))
        << "Island should be enemy";

    gamestate_->get_map()->get_cell({2, 4})->set_player(0);
    gamestate_->get_boat(5)->nb_or = 0;
    ActionDischarge a5(5, {2, 4}, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a5.check(gamestate_))
        << "There shouldn't be enough gold";

    gamestate_->get_boat(5)->nb_or = 4;
    ActionDischarge a6(5, {2, 4}, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a6.check(gamestate_))
        << "There shouldn't be enough gold";

    gamestate_->get_boat(6)->nb_or = 4;
    ActionDischarge a7(6, {2, 4}, 1, 0);
    EXPECT_EQ(BATEAU_ENNEMI, a7.check(gamestate_))
        << "Boat should be enemy";

    gamestate_->get_boat(7)->nb_or = 4;
    ActionDischarge a8(7, {2, 4}, 1, 0);
    EXPECT_EQ(POSITION_INVALIDE, a8.check(gamestate_))
        << "Boat shuldn't be in the same cell than the island";

    gamestate_->get_boat(5)->nb_or = 5;
    ActionDischarge a9(5, {2, 4}, 5, 0);
    EXPECT_EQ(OK, a9.check(gamestate_))
        << "Should be OK";
}

TEST_F(ActionsTest, DischargeTest)
{
    Cell* c = gamestate_->get_map()->get_cell({2, 4});
    c->set_player(0);
    c->set_gold(5);

    gamestate_->get_boat(5)->nb_or = 4;

    ActionDischarge a1(5, {2, 4}, 3, 0);
    a1.apply_on(gamestate_);
    EXPECT_EQ(8, c->get_gold())
        << "Only 2 gold should remain";
    EXPECT_EQ(1, gamestate_->get_boat(5)->nb_or)
        << "Only 1 gold should remain";
}


TEST_F(ActionsTest, ConstructCheckTest)
{
    ActionConstruct a1(BATEAU_GALION, {-1, 0}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a1.check(gamestate_))
        << "Position should be invalid";

    ActionConstruct a2(BATEAU_GALION, {1, TAILLE_TERRAIN}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a2.check(gamestate_))
        << "Position should be invalid";

    ActionConstruct a3(BATEAU_GALION, {0, 0}, 0);
    EXPECT_EQ(ILE_INVALIDE, a3.check(gamestate_))
        << "Island should be invalid";

    gamestate_->get_map()->get_cell({2, 0})->set_player(1);
    ActionConstruct a4(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(ILE_ENNEMIE, a4.check(gamestate_))
        << "Island should be enemy";

    gamestate_->get_map()->get_cell({2, 0})->set_player(0);
    gamestate_->get_map()->get_cell({2, 0})->set_gold(0);
    ActionConstruct a5(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a5.check(gamestate_))
        << "There shouldn't be enough gold";

    gamestate_->get_map()->get_cell({2, 0})->set_gold(GALION_COUT - 1);
    ActionConstruct a6(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a6.check(gamestate_))
        << "There shouldn't be enough gold";

    gamestate_->get_map()->get_cell({2, 0})->set_gold(GALION_COUT);
    ActionConstruct a7(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(OK, a7.check(gamestate_))
        << "Should be OK";

    gamestate_->get_map()->get_cell({2, 0})->set_gold(0);
    ActionConstruct a8(BATEAU_CARAVELLE, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a8.check(gamestate_))
        << "There shouldn't be enough gold";

    gamestate_->get_map()->get_cell({2, 0})->set_gold(CARAVELLE_COUT - 1);
    ActionConstruct a9(BATEAU_CARAVELLE, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a9.check(gamestate_))
        << "There shouldn't be enough gold"; 

    gamestate_->get_map()->get_cell({2, 0})->set_gold(CARAVELLE_COUT);
    ActionConstruct a10(BATEAU_CARAVELLE, {2, 0}, 0);
    EXPECT_EQ(OK, a10.check(gamestate_))
        << "Should be OK";
}

TEST_F(ActionsTest, ConstructTest)
{
    Cell* c = gamestate_->get_map()->get_cell({2, 2});
    c->set_player(0);
    int o = c->get_id_boats().size();
    int p = gamestate_->get_boats().size();

    c->set_gold(CARAVELLE_COUT + 3);
    ActionConstruct a1(BATEAU_CARAVELLE, {2, 2}, 0);
    a1.apply_on(gamestate_);
    EXPECT_EQ(3, c->get_gold())
        << "Only 3 gold should remain";
    EXPECT_EQ(o + 1, c->get_id_boats().size())
        << "No boat created";
    EXPECT_EQ(p + 1, gamestate_->get_boats().size())
        << "No boat created";
}

TEST_F(ActionsTest, ColonizeCheckTest)
{
    ActionColonize a1({-1, 0}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a1.check(gamestate_))
        << "Position should be invalid";

    ActionColonize a2({1, TAILLE_TERRAIN}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a2.check(gamestate_))
        << "Position should be invalid";

    ActionColonize a3({0, 0}, 0);
    EXPECT_EQ(ILE_INVALIDE, a3.check(gamestate_))
        << "Island should be invalid";

    gamestate_->get_map()->get_cell({2, 0})->set_player(0);
    ActionColonize a4({2, 0}, 0);
    EXPECT_EQ(ILE_COLONISEE, a4.check(gamestate_))
        << "Island should be colonized";

    gamestate_->get_map()->get_cell({2, 0})->set_player(-1);
    ActionColonize a5({2, 0}, 0);
    EXPECT_EQ(AUCUNE_CARAVELLE, a5.check(gamestate_))
        << "There shouldn't be any caravelles";

    gamestate_->add_boat({2, 0}, 1, BATEAU_CARAVELLE);
    ActionColonize a6({2, 0}, 0);
    EXPECT_EQ(AUCUNE_CARAVELLE, a6.check(gamestate_))
        << "There shouldn't be any caravelles";

    gamestate_->add_boat({2, 0}, 0, BATEAU_CARAVELLE);
    ActionColonize a7({2, 0}, 0);
    EXPECT_EQ(OK, a7.check(gamestate_))
        << "Should be OK";
}

TEST_F(ActionsTest, ColonizeTest)
{
    gamestate_->get_map()->get_cell({2, 0})->set_player(-1);
    gamestate_->add_boat({2, 0}, 0, BATEAU_CARAVELLE);
    ActionColonize a({2, 0}, 0);
    a.apply_on(gamestate_);
    EXPECT_EQ(0, gamestate_->get_map()->get_cell({2, 0})->get_player())
        << "Island has not beed colonized";
}

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

    c = gamestate_->get_map()->get_cell({4, 3});
    c->set_player(1);

    ActionMove a1(0, {4, 3}, 0); // Origin : 3, 3
    a1.apply_on(gamestate_);

    std::map<int, bateau> boats = gamestate_->get_boats();
    EXPECT_EQ(1, boats.count(0)) << "Boat has disappeared from boat std::map";

    c = gamestate_->get_map()->get_cell({3, 3});
    EXPECT_FALSE(c->exists_boat(0)) << "Still in the original position";

    c = gamestate_->get_map()->get_cell({4, 3});
    EXPECT_TRUE(c->exists_boat(0)) << "Not in the destination position";
    EXPECT_EQ(0, c->get_player()) << "Player hasn't been updated";

    bateau boat = gamestate_->get_boats()[0];
    EXPECT_EQ(boat.pos, (position {4, 3})) << "bateau.pos hasn't been updated";

    c = gamestate_->get_map()->get_cell({2, 2});
    c->set_player(1);
    gamestate_->add_boat({2, 2}, 1, BATEAU_GALION);
    ActionMove a2(0, {2, 2}, 0); // Origin : 4, 3
    a2.apply_on(gamestate_);
    EXPECT_EQ(1, c->get_player()) << "Player shouldn't be updated";
}
