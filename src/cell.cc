#include <map>

#include "constant.hh"
#include "tools.hh"
#include "cell.hh"

Cell::Cell(int y, int x, terrain type)
    : x_(x),
      y_(y),
      type_(type),
      player_(-1)
{
    if (type_ == TERRAIN_ILE || type_ == TERRAIN_VOLCAN)
        gold_ = 0;
    else
        gold_ = -1;
}

terrain Cell::get_type() const
{
    return type_;
}

int Cell::get_gold() const
{
    return gold_;
}

void Cell::set_gold(int gold)
{
    gold_ = gold;
}

int Cell::get_player() const
{
    return player_;
}

void Cell::set_player(int player)
{
    player_ = player;
}

void Cell::add_boat(int boat_id)
{
    boat_ids_.insert(boat_id);
}

bool Cell::remove_boat(int boat_id)
{
    return boat_ids_.erase(boat_id) != 0;
}

bool Cell::exists_boat(int boat_id)
{
    return boat_ids_.count(boat_id);
}

std::set<int>& Cell::get_id_boats()
{
    return boat_ids_;
}
