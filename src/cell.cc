#include <algorithm>

#include "constant.hh"
#include "cell.hh"

Cell::Cell(int y, int x, terrain type)
    : x_(x),
      y_(y),
      type_(type),
      or_(-1),
      player_(-1)
{
}

terrain Cell::get_terrain() const
{
    return type_;
}

int Cell::get_or() const
{
    return or_;
}

int Cell::get_player() const
{
    return player_;
}

void Cell::bateau_add(int b)
{
    id_bateaux_.push_back(b);
}

bool Cell::bateau_remove(int id)
{
    for (int i = 0; i < id_bateaux_.size(); i++)
    {
        if (id_bateaux_[i] == id)
        {
            id_bateaux_.erase(id_bateaux_.begin() + i);
            return true;
        }
    }
    return false;
}

bool Cell::bateau_exists(int id)
{
    return std::find(id_bateaux_.begin(), id_bateaux_.end(), id) !=
        id_bateaux_.end();
}
