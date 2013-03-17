#include <gtest/gtest.h>

#include <map>
#include <utils/log.hh>
#include "../constant.hh"
#include "../cell.hh"

class CellTest : public ::testing::Test
{
    protected:
    virtual void SetUp()
    {
        utils::Logger::get().level() = utils::Logger::DEBUG_LEVEL;

        sea_cell = new Cell(5, 10, TERRAIN_MER);
        island_cell = new Cell(5, 10, TERRAIN_ILE);
        volcano_cell = new Cell(5, 10, TERRAIN_VOLCAN);
        decoy_cell = new Cell(5, 10, TERRAIN_ERREUR);

        pos_ = {10, 5};
    }

    std::map<int, bateau> make_boats(
        int j1_galions, int j1_caravelles, int j2_galions, int j2_caravelles,
        Cell* cell_)
    {
        std::map<int, bateau> boats_;

        int i = 0;
        while (i < j1_galions)
        {
            boats_[i] = {i, pos_, 1, BATEAU_GALION, 0, false};
            cell_->add_boat(i);
            i++;
        }
        while (i < j1_galions + j1_caravelles)
        {
            boats_[i] = {i, pos_, 1, BATEAU_CARAVELLE, 10, false};
            cell_->add_boat(i);
            i++;
        }
        while (i < j1_galions + j1_caravelles + j2_galions)
        {
            boats_[i] = {i, pos_, 2, BATEAU_GALION, 0, false};
            cell_->add_boat(i);
            i++;
        }
        while (i < j1_galions + j1_caravelles + j2_galions + j2_caravelles)
        {
            boats_[i] = {i, pos_, 2, BATEAU_CARAVELLE, 10, false};
            cell_->add_boat(i);
            i++;
        }

        return boats_;
    }

    int count_boats(std::map<int, bateau> boats, Cell* cell, int player_id)
    {
        std::set<int> boats_ids = cell->get_id_boats();

        int count = 0;
        for (auto id : boats_ids)
        {
            if (boats.find(id) == boats.end())
                return(-1);
            else if (boats.find(id)->second.joueur == player_id)
                count++;
        }

        return(count);
    }

    int count_gold(std::map<int, bateau> boats, Cell* cell, int player_id)
    {
        std::set<int> boats_ids = cell->get_id_boats();

        int count = 0;
        for (auto id : boats_ids)
        {
            bateau boat = boats.find(id)->second;
            if (boat.btype == BATEAU_CARAVELLE && boat.joueur == player_id)
                count += boat.nb_or;
        }

        return(count);
    }

    position pos_;

    Cell* sea_cell;
    Cell* island_cell;
    Cell* volcano_cell;
    Cell* decoy_cell; // For using make_boats without adding boats to a cell

};


TEST_F(CellTest, AddRemoveBoats)
{
    std::map<int, bateau> boats_ = make_boats(1, 0, 0, 0, decoy_cell);

    EXPECT_FALSE(sea_cell->exists_boat(0)) << "No boat at all at start";
    EXPECT_FALSE(sea_cell->exists_boat(1)) << "No boat at all at start";

    sea_cell->add_boat(0);

    INFO("add_boat (0)");
    EXPECT_TRUE(sea_cell->exists_boat(0)) << "A Galion should be there";
    EXPECT_EQ(BATEAU_GALION, boats_[0].btype) << "A Galion should be there";
    EXPECT_FALSE(sea_cell->exists_boat(1)) << "No boat should be here";

    sea_cell->remove_boat(0);
    sea_cell->add_boat(1);

    INFO("remove_boat (galion)");
    EXPECT_FALSE(sea_cell->exists_boat(0)) << "No boat should be here";

    INFO("add_boat (caravelle)");
    EXPECT_TRUE(sea_cell->exists_boat(1)) << "A Caravelle should be there";
    EXPECT_EQ(BATEAU_CARAVELLE, boats_[1].btype) << "A Caravelle should be there";
}

TEST_F(CellTest, SeaFight1)
{
    // One versus one, player 1 attacking
    std::map<int, bateau> boats_ = make_boats(1, 0, 1, 0, sea_cell);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    EXPECT_EQ(2, boats_.size());

    sea_cell-> resolve_fight(boats_, 1);

    INFO("resolve_fight");
    EXPECT_EQ(1, boats_.size());

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(boats_, sea_cell, 1)) << "Winner's boat must be remaining";

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 2)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(boats_, sea_cell, 2)) << "Loser's boat must be destroyed";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(CellTest, SeaFight2)
{
    // One versus one, player 1 attacking
    std::map<int, bateau> boats_ = make_boats(1, 0, 1, 0, sea_cell);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    EXPECT_EQ(2, boats_.size());

    sea_cell-> resolve_fight(boats_, 2);

    INFO("resolve_fight");
    EXPECT_EQ(1, boats_.size());

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(boats_, sea_cell, 1)) << "Loser's boat must be destroyed";

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 2)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(boats_, sea_cell, 2)) << "Winner's boat must be remaining";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(CellTest, SeaFight3)
{
    // Player 1 win
    std::map<int, bateau> boats_ = make_boats(10, 0, 5, 0, sea_cell);

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    sea_cell->resolve_fight(boats_, 1);

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(6, count_boats(boats_, sea_cell, 1)) << "6 boats should remain for the winner";

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 2)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(boats_, sea_cell, 2)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(CellTest, SeaFight4)
{
    // Player 2 win
    std::map<int, bateau> boats_ = make_boats(8, 0, 9, 0, sea_cell);

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    sea_cell->resolve_fight(boats_, 1);

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 2)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(2, count_boats(boats_, sea_cell, 2)) << "2 boats should remain for the winner";

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(boats_, sea_cell, 1)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(CellTest, SeaFight5)
{
    // Exaequo, player 2 wins
    std::map<int, bateau> boats_ = make_boats(20, 0, 20, 0, sea_cell);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    EXPECT_EQ(40, boats_.size());

    sea_cell-> resolve_fight(boats_, 2);

    INFO("resolve_fight");
    EXPECT_EQ(1, boats_.size());

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 2)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(boats_, sea_cell, 2)) << "1 boat should remain for the winner";

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(boats_, sea_cell, 1)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(CellTest, SeaFight6)
{
    // Exaequo, player 1 wins
    std::map<int, bateau> boats_ = make_boats(20, 0, 20, 0, sea_cell);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    EXPECT_EQ(40, boats_.size());

    sea_cell-> resolve_fight(boats_, 1);

    INFO("resolve_fight");
    EXPECT_EQ(1, boats_.size());

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 1)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(1, count_boats(boats_, sea_cell, 1)) << "1 boat should remain for the winner";

    EXPECT_NE(-1, count_boats(boats_, sea_cell, 2)) << "The wrong boats were deleted from boats std::map<int, bateau>";
    EXPECT_EQ(0, count_boats(boats_, sea_cell, 2)) << "No boat should remain for the loser";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(CellTest, IslandFight1)
{
    // Winning island attack
    std::map<int, bateau> boats_ = make_boats(20, 0, 15, 0, island_cell);
    island_cell->set_player(2);

    EXPECT_EQ(2, island_cell->get_player()) << "Before the fight, island belongs to 2";
    island_cell->resolve_fight(boats_, 1);
    EXPECT_EQ(1, island_cell->get_player()) << "After the fight, island belongs to 1";
}

TEST_F(CellTest, IslandFight2)
{
    // Losed island attack
    std::map<int, bateau> boats_ = make_boats(10, 0, 15, 0, island_cell);
    island_cell->set_player(2);

    EXPECT_EQ(2, island_cell->get_player()) << "Before the fight, island belongs to 2";
    island_cell->resolve_fight(boats_, 1);
    EXPECT_EQ(2, island_cell->get_player()) << "After the fight, island still belongs to 2";
}

TEST_F(CellTest, IslandFight3)
{
    // Winning island attack
    std::map<int, bateau> boats_ = make_boats(15, 0, 20, 0, island_cell);
    island_cell->set_player(1);

    EXPECT_EQ(1, island_cell->get_player()) << "Before the fight, island belongs to 1";
    island_cell->resolve_fight(boats_, 2);
    EXPECT_EQ(2, island_cell->get_player()) << "After the fight, island belongs to 2";
}

TEST_F(CellTest, IslandFight4)
{
    // Losed island attack
    std::map<int, bateau> boats_ = make_boats(15, 0, 10, 0, island_cell);
    island_cell->set_player(1);

    EXPECT_EQ(1, island_cell->get_player()) << "Before the fight, island belongs to 2";
    island_cell->resolve_fight(boats_, 2);
    EXPECT_EQ(1, island_cell->get_player()) << "After the fight, island still belongs to 2";
}

TEST_F(CellTest, GoldSea)
{
    std::map<int, bateau> boats_ = make_boats(15, 3, 10, 2, sea_cell);

    EXPECT_EQ(30, count_gold(boats_, sea_cell, 1)) << "Player 1 should start with 30 gold";
    EXPECT_EQ(20, count_gold(boats_, sea_cell, 2)) << "Player 2 should start with 20 gold";
    sea_cell->resolve_fight(boats_, 1);
    EXPECT_EQ(50, count_gold(boats_, sea_cell, 1)) << "Player 1 should end up with 50 gold";
    EXPECT_EQ(0, count_gold(boats_, sea_cell, 2)) << "Player 2 should end up with no gold";
}

TEST_F(CellTest, GoldIsland)
{
    std::map<int, bateau> boats_ = make_boats(15, 3, 20, 2, island_cell);

    EXPECT_EQ(30, count_gold(boats_, island_cell, 1)) << "Player 1 should start with 30 gold";
    EXPECT_EQ(20, count_gold(boats_, island_cell, 2)) << "Player 2 should start with 20 gold";
    island_cell->resolve_fight(boats_, 2);
    EXPECT_EQ(0, count_gold(boats_, island_cell, 1)) << "Player 1 should end up with no gold";
    EXPECT_EQ(50, count_gold(boats_, island_cell, 2)) << "Player 2 should end up with 50 gold";
}

TEST_F(CellTest, GoldVolcano)
{
    std::map<int, bateau> boats_ = make_boats(15, 3, 10, 2, volcano_cell);

    EXPECT_EQ(30, count_gold(boats_, volcano_cell, 1)) << "Player 1 should start with 30 gold";
    EXPECT_EQ(20, count_gold(boats_, volcano_cell, 2)) << "Player 2 should start with 20 gold";
    volcano_cell->resolve_fight(boats_, 2);
    EXPECT_EQ(50, count_gold(boats_, volcano_cell, 1)) << "Player 1 should end up with 50 gold";
    EXPECT_EQ(0, count_gold(boats_, volcano_cell, 2)) << "Player 2 should end up with no gold";
}
