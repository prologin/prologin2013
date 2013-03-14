#ifndef CELL_HH_
#define CELL_HH_

#include <set>
#include <map>

#include "constant.hh"

class Cell
{
    public:
        Cell(int y, int x, terrain type);

        terrain get_type() const;
        int get_gold() const;
        int get_player() const;

        void add_boat(int boat_id);
        bool remove_boat(int boat_id);
        bool exists_boat(int boat_id);

        void resolve_fight(std::map<int, bateau>& boats);

    private:
        int x_;
        int y_;
        terrain type_;
        int gold_;
        int player_;
        std::set<int> boat_ids_;
};

#endif // !CELL_HH_
