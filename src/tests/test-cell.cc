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
