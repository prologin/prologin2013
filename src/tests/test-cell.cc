#include <gtest/gtest.h>

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
            boats_[i] = {i, pos_, 1, BATEAU_CARAVELLE, 0, false};
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
            boats_[i] = {i, pos_, 2, BATEAU_CARAVELLE, 0, false};
            cell_->add_boat(i);
            i++;
        }

        return boats_;
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
    std::map<int, bateau> boats_ = make_boats(1, 0, 1, 0, sea_cell);

    INFO("get_player");
    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    EXPECT_EQ(2, boats_.size());

    sea_cell-> resolve_fight(boats_, 2);

    INFO("resolve_fight");
    EXPECT_EQ(1, boats_.size());
    EXPECT_TRUE(sea_cell->exists_boat(1)) << "Winner's boat must be remaining";
    EXPECT_FALSE(sea_cell->exists_boat(0)) << "Loser's boat must be destroyed";

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}

TEST_F(CellTest, SeaFight2)
{
    std::map<int, bateau> boats_ = make_boats(10, 0, 5, 0, sea_cell);

    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should belong to nobody before first fight";

    sea_cell->resolve_fight(boats_, 1);

    for(int i=0; i <=5; i++)
        EXPECT_TRUE(sea_cell->exists_boat(i)) << "Winner's boat number " << i << " must be remaining";
    for(int i=6; i <=14; i++)
        EXPECT_FALSE(sea_cell->exists_boat(i)) << "Boat number " << i << " must be destroyed";


    EXPECT_EQ(-1, sea_cell->get_player()) << "Cell should still belong to nobody after the fight";
}
