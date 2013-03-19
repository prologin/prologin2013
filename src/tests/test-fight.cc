#include <sstream>
#include <iostream>

#include <gtest/gtest.h>

#include <utils/log.hh>
#include "../constant.hh"
#include "../map.hh"
#include "../cell.hh"
#include "../game.hh"

class GameTest : public ::testing::Test
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

    }

    int count_boats(position pos, int player_id)
    {
        Cell* cell = gamestate_->get_map()->get_cell(pos);
        std::set<int> boats_ids = cell->get_id_boats();

        int count = 0;
        for (auto id : boats_ids)
        {
            bateau* boat = gamestate_->get_boat(id);
            if (boat == NULL)
                return(-1);
            else if (boat->joueur == player_id)
                count++;
        }

        return(count);
    }

    int count_gold(position pos, int player_id)
    {
        Cell* cell = gamestate_->get_map()->get_cell(pos);
        std::set<int> boats_ids = cell->get_id_boats();

        int count = 0;
        for (auto id : boats_ids)
        {
            bateau* boat = gamestate_->get_boat(id);
            if (boat->btype == BATEAU_CARAVELLE && boat->joueur == player_id)
            count += boat->nb_or;
        }

        return(count);
    }

    std::stringstream f;
    Map* map_;
    GameState* gamestate_;
};


TEST_F(GameTest, SeaFight1)
{
    // One versus zero, player 1 attacking
    position pos = {4, 4};
    gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0
    Cell* sea_cell = gamestate_->get_map()->get_cell(pos);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    EXPECT_EQ(1, (int) sea_cell->get_id_boats().size());

    gamestate_->resolve_fight(pos, 0);

    INFO("resolve_fight");
    EXPECT_EQ(1, (int) sea_cell->get_id_boats().size());

    EXPECT_NE(-1, count_boats(pos, 0)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(pos, 0)) << "Winner's boat must be remaining";

    EXPECT_NE(-1, count_boats(pos, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(pos, 1)) << "Loser's boat must be destroyed";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(GameTest, SeaFight2)
{
    // One versus one, player 1 attacking
    position pos = {4, 4};
    gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0
    gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0
    Cell* sea_cell = gamestate_->get_map()->get_cell(pos);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Sea cell should belong to nobody";

    EXPECT_EQ(2, (int) sea_cell->get_id_boats().size());

    gamestate_->resolve_fight(pos, 1);

    INFO("resolve_fight");
    EXPECT_EQ(1, (int) sea_cell->get_id_boats().size());

    EXPECT_NE(-1, count_boats(pos, 0)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(pos, 0)) << "Loser's boat must be destroyed";

    EXPECT_NE(-1, count_boats(pos, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(pos, 1)) << "Winner's boat must be remaining";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Sea cell should still belong to nobody after the fight";
}

TEST_F(GameTest, SeaFight3)
{
    // Player 1 win
    position pos = {4, 4};

    for(int i = 0; i < 10; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 5; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* sea_cell = gamestate_->get_map()->get_cell(pos);

    EXPECT_EQ(-1, sea_cell->get_player()) << "Sea cell should belong to nobody";

    gamestate_->resolve_fight(pos, 0);

    EXPECT_NE(-1, count_boats(pos, 0)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(6, count_boats(pos, 0)) << "6 boats should remain for the winner";

    EXPECT_NE(-1, count_boats(pos, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(pos, 1)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(GameTest, SeaFight4)
{
    // Player 2 win
    position pos = {4, 4};

    for(int i = 0; i < 8; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 9; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* sea_cell = gamestate_->get_map()->get_cell(pos);

    EXPECT_EQ(-1, sea_cell->get_player()) << "Sea cell should belong to nobody";

    gamestate_->resolve_fight(pos, 0);

    EXPECT_NE(-1, count_boats(pos, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(2, count_boats(pos, 1)) << "2 boats should remain for the winner";

    EXPECT_NE(-1, count_boats(pos, 0)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(pos, 0)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(GameTest, SeaFight5)
{
    // Exaequo, player 2 wins
    position pos = {4, 4};

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* sea_cell = gamestate_->get_map()->get_cell(pos);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Sea cell should belong to nobody";

    EXPECT_EQ(40, (int) sea_cell->get_id_boats().size());

    gamestate_->resolve_fight(pos, 1);

    INFO("resolve_fight");
    EXPECT_EQ(1, (int) sea_cell->get_id_boats().size());

    EXPECT_NE(-1, count_boats(pos, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(pos, 1)) << "1 boat should remain for the winner";

    EXPECT_NE(-1, count_boats(pos, 0)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(pos, 0)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(GameTest, SeaFight6)
{
    // Exaequo, player 1 wins
    position pos = {4, 4};

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* sea_cell = gamestate_->get_map()->get_cell(pos);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    EXPECT_EQ(40, (int) sea_cell->get_id_boats().size());

    gamestate_->resolve_fight(pos, 0);

    INFO("resolve_fight");
    EXPECT_EQ(1, (int) sea_cell->get_id_boats().size());

    EXPECT_NE(-1, count_boats(pos, 0)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(pos, 0)) << "1 boat should remain for the winner";

    EXPECT_NE(-1, count_boats(pos, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(pos, 1)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(GameTest, IslandFight1)
{
    // Ex-aequo island attack
    position pos = {3, 3};

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* island_cell = gamestate_->get_map()->get_cell(pos);

    island_cell->set_player(1);

    EXPECT_EQ(1, island_cell->get_player()) << "Before the fight, island belongs to 1";
    gamestate_->resolve_fight(pos, 0);
    EXPECT_EQ(1, island_cell->get_player()) << "After the fight, still island belongs to 1";
}

TEST_F(GameTest, IslandFight2)
{
    // Losed island attack
    position pos = {3, 3};

    for(int i = 0; i < 10; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 15; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* island_cell = gamestate_->get_map()->get_cell(pos);
    island_cell->set_player(1);

    EXPECT_EQ(1, island_cell->get_player()) << "Before the fight, island belongs to 1";
    gamestate_->resolve_fight(pos, 0);
    EXPECT_EQ(1, island_cell->get_player()) << "After the fight, island still belongs to 1";
}

TEST_F(GameTest, IslandFight3)
{
    // Winning island attack
    position pos = {3, 3};

    for(int i = 0; i < 15; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* island_cell = gamestate_->get_map()->get_cell(pos);
    island_cell->set_player(0);

    EXPECT_EQ(0, island_cell->get_player()) << "Before the fight, island belongs to 0";
    gamestate_->resolve_fight(pos, 1);
    EXPECT_EQ(1, island_cell->get_player()) << "After the fight, island belongs to 1";
}

TEST_F(GameTest, IslandFight4)
{
    // Losed island attack
    position pos = {3, 3};

    for(int i = 0; i < 15; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0

    for(int i = 0; i < 10; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0

    Cell* island_cell = gamestate_->get_map()->get_cell(pos);
    island_cell->set_player(0);

    EXPECT_EQ(0, island_cell->get_player()) << "Before the fight, island belongs to 0";
    gamestate_->resolve_fight(pos, 1);
    EXPECT_EQ(0, island_cell->get_player()) << "After the fight, island still belongs to 0";
}

TEST_F(GameTest, GoldSea)
{
    position pos = {4, 4};

    for(int i = 0; i < 15; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0
    for(int i = 0; i < 3; i++)
    {
        gamestate_->add_boat(pos, 0, BATEAU_CARAVELLE); // 0
        gamestate_->get_boat(15+i)->nb_or = 10;
    }

    for(int i = 0; i < 10; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0
    for(int i = 0; i < 2; i++)
    {
        gamestate_->add_boat(pos, 1, BATEAU_CARAVELLE); // 0
        gamestate_->get_boat(15+3+10+i)->nb_or = 10;
    }

    EXPECT_EQ(30, count_gold(pos, 0)) << "Player 0 should start with 30 gold";
    EXPECT_EQ(20, count_gold(pos, 1)) << "Player 1 should start with 20 gold";
    gamestate_->resolve_fight(pos, 0);
    EXPECT_EQ(50, count_gold(pos, 0)) << "Player 0 should end up with 50 gold";
    EXPECT_EQ(0, count_gold(pos, 1)) << "Player 1 should end up with no gold";
}

TEST_F(GameTest, GoldIsland)
{
    position pos = {3, 3};

    for(int i = 0; i < 15; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0
    for(int i = 0; i < 3; i++)
    {
        gamestate_->add_boat(pos, 0, BATEAU_CARAVELLE); // 0
        gamestate_->get_boat(15+i)->nb_or = 10;
    }

    for(int i = 0; i < 20; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0
    for(int i = 0; i < 2; i++)
    {
        gamestate_->add_boat(pos, 1, BATEAU_CARAVELLE); // 0
        gamestate_->get_boat(15+3+20+i)->nb_or = 10;
    }

    EXPECT_EQ(30, count_gold(pos, 0)) << "Player 0 should start with 30 gold";
    EXPECT_EQ(20, count_gold(pos, 1)) << "Player 1 should start with 20 gold";
    gamestate_->resolve_fight(pos, 1);
    EXPECT_EQ(0, count_gold(pos, 0)) << "Player 0 should end up with no gold";
    EXPECT_EQ(50, count_gold(pos, 1)) << "Player 1 should end up with 50 gold";
}

TEST_F(GameTest, GoldVolcano)
{
    position pos = {4, 4};

    for(int i = 0; i < 15; i++)
        gamestate_->add_boat(pos, 0, BATEAU_GALION); // 0
    for(int i = 0; i < 3; i++)
    {
        gamestate_->add_boat(pos, 0, BATEAU_CARAVELLE); // 0
        gamestate_->get_boat(15+i)->nb_or = 10;
    }

    for(int i = 0; i < 10; i++)
        gamestate_->add_boat(pos, 1, BATEAU_GALION); // 0
    for(int i = 0; i < 2; i++)
    {
        gamestate_->add_boat(pos, 1, BATEAU_CARAVELLE); // 0
        gamestate_->get_boat(15+3+10+i)->nb_or = 10;
    }

    EXPECT_EQ(30, count_gold(pos, 0)) << "Player 0 should start with 30 gold";
    EXPECT_EQ(20, count_gold(pos, 1)) << "Player 1 should start with 20 gold";
    gamestate_->resolve_fight(pos, 1);
    EXPECT_EQ(50, count_gold(pos, 0)) << "Player 0 should end up with 50 gold";
    EXPECT_EQ(0, count_gold(pos, 1)) << "Player 1 should end up with no gold";
}
