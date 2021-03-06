#include <iostream>
#include <sstream>

#include <gtest/gtest.h>

#include "../cell.hh"
#include "../constant.hh"
#include "../game.hh"
#include "../map.hh"
#include <utils/log.hh>

#include "../action-charge.hh"
#include "../action-colonize.hh"
#include "../action-construct.hh"
#include "../action-discharge.hh"
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

        rules::Players players;
        players.add(std::make_shared<rules::Player>(0, rules::PLAYER));
        players.add(std::make_shared<rules::Player>(1, rules::PLAYER));

        st = new GameState(map_, players);
        st->init();

        st->add_boat({3, 3}, 0, BATEAU_GALION); // 0
        st->get_boat(0)->deplacable = true;

        st->add_boat({3, 6}, 1, BATEAU_GALION); // 1
        st->get_boat(1)->deplacable = true;

        st->add_boat({5, 8}, 0, BATEAU_GALION); // 2
        st->get_boat(2)->deplacable = false;

        st->add_boat({31, 31}, 0, BATEAU_GALION); // 3
        st->get_boat(3)->deplacable = true;

        st->add_boat({3, 3}, 0, BATEAU_CARAVELLE); // 4
        st->get_boat(4)->deplacable = true;

        st->add_boat({2, 4}, 0, BATEAU_CARAVELLE); // 5
        st->get_boat(4)->deplacable = true;

        st->add_boat({2, 4}, 1, BATEAU_CARAVELLE); // 6
        st->get_boat(4)->deplacable = true;

        st->add_boat({2, 5}, 0, BATEAU_CARAVELLE); // 7
        st->get_boat(4)->deplacable = true;
    }

    virtual void TearDown() { delete st; }

    std::stringstream f;
    Map* map_;
    GameState* st;
};

TEST_F(ActionsTest, ChargeCheckTest)
{
    st->get_map()->get_cell({2, 4})->set_player(1);
    ActionCharge a4(5, 1, 0);
    EXPECT_EQ(ILE_ENNEMIE, a4.check(*st)) << "Island should be enemy";

    st->get_map()->get_cell({2, 4})->set_player(0);
    st->get_map()->get_cell({2, 4})->set_gold(0);
    ActionCharge a5(5, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a5.check(*st))
        << "There shouldn't be enough gold";

    st->get_map()->get_cell({2, 4})->set_gold(4);
    ActionCharge a6(5, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a6.check(*st))
        << "There shouldn't be enough gold";

    st->get_map()->get_cell({2, 4})->set_gold(5);
    ActionCharge a7(6, 1, 0);
    EXPECT_EQ(BATEAU_ENNEMI, a7.check(*st)) << "Boat should be enemy";

    st->get_map()->get_cell({2, 4})->set_gold(5);
    ActionCharge a9(5, 5, 0);
    EXPECT_EQ(OK, a9.check(*st)) << "Should be OK";
}

TEST_F(ActionsTest, ChargeTest)
{
    Cell* c = st->get_map()->get_cell({2, 4});
    c->set_player(0);

    c->set_gold(5);
    ActionCharge a1(5, 3, 0);
    a1.apply(st);
    EXPECT_EQ(2, c->get_gold()) << "Only 2 gold should remain";
    EXPECT_EQ(3, st->get_boat(5)->nb_or) << "Only 3 gold should remain";
}

TEST_F(ActionsTest, DischargeCheckTest)
{
    st->get_map()->get_cell({2, 4})->set_player(1);
    ActionDischarge a4(5, 1, 0);
    EXPECT_EQ(ILE_ENNEMIE, a4.check(*st)) << "Island should be enemy";

    st->get_map()->get_cell({2, 4})->set_player(0);
    st->get_boat(5)->nb_or = 0;
    ActionDischarge a5(5, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a5.check(*st))
        << "There shouldn't be enough gold";

    st->get_boat(5)->nb_or = 4;
    ActionDischarge a6(5, 5, 0);
    EXPECT_EQ(OR_INSUFFISANT, a6.check(*st))
        << "There shouldn't be enough gold";

    st->get_boat(6)->nb_or = 4;
    ActionDischarge a7(6, 1, 0);
    EXPECT_EQ(BATEAU_ENNEMI, a7.check(*st)) << "Boat should be enemy";

    st->get_boat(5)->nb_or = 5;
    ActionDischarge a9(5, 5, 0);
    EXPECT_EQ(OK, a9.check(*st)) << "Should be OK";
}

TEST_F(ActionsTest, DischargeTest)
{
    Cell* c = st->get_map()->get_cell({2, 4});
    c->set_player(0);
    c->set_gold(5);

    st->get_boat(5)->nb_or = 4;

    ActionDischarge a1(5, 3, 0);
    a1.apply(st);
    EXPECT_EQ(8, c->get_gold()) << "Only 2 gold should remain";
    EXPECT_EQ(1, st->get_boat(5)->nb_or) << "Only 1 gold should remain";
}

TEST_F(ActionsTest, ConstructCheckTest)
{
    ActionConstruct a1(BATEAU_GALION, {-1, 0}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a1.check(*st)) << "Position should be invalid";

    ActionConstruct a2(BATEAU_GALION, {1, TAILLE_TERRAIN}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a2.check(*st)) << "Position should be invalid";

    ActionConstruct a3(BATEAU_GALION, {0, 0}, 0);
    EXPECT_EQ(ILE_INVALIDE, a3.check(*st)) << "Island should be invalid";

    st->get_map()->get_cell({2, 0})->set_player(1);
    ActionConstruct a4(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(ILE_ENNEMIE, a4.check(*st)) << "Island should be enemy";

    st->get_map()->get_cell({2, 0})->set_player(0);
    st->get_map()->get_cell({2, 0})->set_gold(0);
    ActionConstruct a5(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a5.check(*st))
        << "There shouldn't be enough gold";

    st->get_map()->get_cell({2, 0})->set_gold(GALION_COUT - 1);
    ActionConstruct a6(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a6.check(*st))
        << "There shouldn't be enough gold";

    st->get_map()->get_cell({2, 0})->set_gold(GALION_COUT);
    ActionConstruct a7(BATEAU_GALION, {2, 0}, 0);
    EXPECT_EQ(OK, a7.check(*st)) << "Should be OK";

    st->get_map()->get_cell({2, 0})->set_gold(0);
    ActionConstruct a8(BATEAU_CARAVELLE, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a8.check(*st))
        << "There shouldn't be enough gold";

    st->get_map()->get_cell({2, 0})->set_gold(CARAVELLE_COUT - 1);
    ActionConstruct a9(BATEAU_CARAVELLE, {2, 0}, 0);
    EXPECT_EQ(OR_INSUFFISANT, a9.check(*st))
        << "There shouldn't be enough gold";

    st->get_map()->get_cell({2, 0})->set_gold(CARAVELLE_COUT);
    ActionConstruct a10(BATEAU_CARAVELLE, {2, 0}, 0);
    EXPECT_EQ(OK, a10.check(*st)) << "Should be OK";
}

TEST_F(ActionsTest, ConstructTest)
{
    Cell* c = st->get_map()->get_cell({2, 2});
    c->set_player(0);
    int o = c->get_id_boats().size();

    c->set_gold(CARAVELLE_COUT + 3);
    ActionConstruct a1(BATEAU_CARAVELLE, {2, 2}, 0);
    a1.apply(st);
    EXPECT_EQ(3, c->get_gold()) << "Only 3 gold should remain";
    EXPECT_EQ(o + 1, (int)c->get_id_boats().size()) << "No boat created";
}

TEST_F(ActionsTest, ColonizeCheckTest)
{
    ActionColonize a1({-1, 0}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a1.check(*st)) << "Position should be invalid";

    ActionColonize a2({1, TAILLE_TERRAIN}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a2.check(*st)) << "Position should be invalid";

    ActionColonize a3({0, 0}, 0);
    EXPECT_EQ(ILE_INVALIDE, a3.check(*st)) << "Island should be invalid";

    st->get_map()->get_cell({2, 0})->set_player(0);
    ActionColonize a4({2, 0}, 0);
    EXPECT_EQ(ILE_COLONISEE, a4.check(*st)) << "Island should be colonized";

    st->get_map()->get_cell({2, 0})->set_player(-1);
    ActionColonize a5({2, 0}, 0);
    EXPECT_EQ(AUCUNE_CARAVELLE, a5.check(*st))
        << "There shouldn't be any caravelles";

    st->add_boat({2, 0}, 1, BATEAU_CARAVELLE);
    ActionColonize a6({2, 0}, 0);
    EXPECT_EQ(AUCUNE_CARAVELLE, a6.check(*st))
        << "There shouldn't be any caravelles";

    st->add_boat({2, 0}, 0, BATEAU_CARAVELLE);
    ActionColonize a7({2, 0}, 0);
    EXPECT_EQ(OK, a7.check(*st)) << "Should be OK";
}

TEST_F(ActionsTest, ColonizeTest)
{
    st->get_map()->get_cell({2, 0})->set_player(-1);
    st->add_boat({2, 0}, 0, BATEAU_CARAVELLE);
    ActionColonize a({2, 0}, 0);
    a.apply(st);
    EXPECT_EQ(0, st->get_map()->get_cell({2, 0})->get_player())
        << "Island has not beed colonized";
}

TEST_F(ActionsTest, MoveCheckTest)
{
    ActionMove a1(672324, {3, 4}, 0);
    EXPECT_EQ(ID_INVALIDE, a1.check(*st)) << "ID should be invalid";

    ActionMove a2(1, {3, 4}, 0);
    EXPECT_EQ(BATEAU_ENNEMI, a2.check(*st)) << "Boat should be enemy";

    ActionMove a3(2, {3, 4}, 0);
    EXPECT_EQ(NON_DEPLACABLE, a3.check(*st)) << "Boat shouldn't be movable";

    ActionMove a6(3, {TAILLE_TERRAIN, 31}, 0);
    EXPECT_EQ(POSITION_INVALIDE, a6.check(*st))
        << "Destination position should be invalid";

    ActionMove a7(0, {3 + GALION_DEPLACEMENT, 3}, 0);
    EXPECT_EQ(OK, a7.check(*st)) << "Should be OK";

    ActionMove a8(0, {3, 3 + GALION_DEPLACEMENT}, 0);
    EXPECT_EQ(OK, a8.check(*st)) << "Should be OK";

    ActionMove a9(0, {3 + 2, 3 + 2}, 0);
    EXPECT_EQ(OK, a9.check(*st)) << "Should be OK";

    ActionMove a10(0, {3 + GALION_DEPLACEMENT, 3 + GALION_DEPLACEMENT}, 0);
    EXPECT_EQ(TROP_LOIN, a10.check(*st)) << "Destination should be too far";

    ActionMove a11(0, {3 + GALION_DEPLACEMENT + 1, 3}, 0);
    EXPECT_EQ(TROP_LOIN, a11.check(*st)) << "Destination should be too far";

    ActionMove a12(4, {3 + CARAVELLE_DEPLACEMENT, 3}, 0);
    EXPECT_EQ(OK, a7.check(*st)) << "Should be OK";

    ActionMove a13(4, {3, 3 + CARAVELLE_DEPLACEMENT}, 0);
    EXPECT_EQ(OK, a8.check(*st)) << "Should be OK";

    ActionMove a14(4, {3 + 2, 3 + 2}, 0);
    EXPECT_EQ(OK, a9.check(*st)) << "Should be OK";

    ActionMove a15(4, {3 + CARAVELLE_DEPLACEMENT, 3 + CARAVELLE_DEPLACEMENT},
                   0);
    EXPECT_EQ(TROP_LOIN, a10.check(*st)) << "Destination should be too far";

    ActionMove a16(4, {3 + CARAVELLE_DEPLACEMENT + 1, 3}, 0);
    EXPECT_EQ(TROP_LOIN, a11.check(*st)) << "Destination should be too far";
}

TEST_F(ActionsTest, MoveTest)
{
    Cell* c;

    c = st->get_map()->get_cell({3, 3});
    EXPECT_TRUE(c->exists_boat(0)) << "Not in the original position";

    c = st->get_map()->get_cell({4, 3});
    c->set_player(1);

    ActionMove a1(0, {4, 3}, 0); // Origin : 3, 3
    a1.apply(st);

    EXPECT_FALSE(st->get_boat(0) == NULL)
        << "Boat has disappeared from boat std::map";

    c = st->get_map()->get_cell({3, 3});
    EXPECT_FALSE(c->exists_boat(0)) << "Still in the original position";

    c = st->get_map()->get_cell({4, 3});
    EXPECT_TRUE(c->exists_boat(0)) << "Not in the destination position";

    bateau* boat = st->get_boat(0);
    EXPECT_EQ(boat->pos, (position{4, 3})) << "bateau.pos hasn't been updated";
}
