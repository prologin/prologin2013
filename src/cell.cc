#include "constant.hh"
#include "cell.hh"

Cell::Cell(int y, int x, terrain type)
    : x_(x),
      y_(y),
      type_(type),
      gold_(-1),
      player_(-1)
{
}

terrain Cell::get_type() const
{
    return type_;
}

int Cell::get_gold() const
{
    return gold_;
}

int Cell::get_player() const
{
    return player_;
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
    return boat_ids_.find(boat_id) != boat_ids_.end();
}

void Cell::resolve_fight()
{
}
