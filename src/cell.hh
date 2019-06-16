#ifndef CELL_HH_
#define CELL_HH_

#include <map>
#include <set>

#include "constant.hh"

class Cell
{
public:
    Cell(terrain type);

    terrain get_type() const;

    int get_gold() const;
    void set_gold(int gold);

    int get_player() const;
    void set_player(int id_player);

    void add_boat(int boat_id);
    bool remove_boat(int boat_id);
    bool exists_boat(int boat_id);
    std::set<int>& get_id_boats();

private:
    terrain type_;
    int gold_;
    int player_;
    std::set<int> boat_ids_;
};

#endif // !CELL_HH_
