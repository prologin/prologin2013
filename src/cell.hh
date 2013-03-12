#ifndef CELL_HH_
#define CELL_HH_

#include <set>
#include "constant.hh"

class Cell
{
    public:
        Cell(int y, int x, terrain type);

        terrain getType() const;
        int getGold() const;
        int getPlayer() const;

        void addBoat(int boatId);
        bool removeBoat(int boatId);
        bool existsBoat(int boatId);

    private:
        int x_;
        int y_;
        terrain type_;
        int gold_;
        int player_;
        std::set<int> boatIds_;
};

#endif // !CELL_HH_
