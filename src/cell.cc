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

terrain Cell::getType() const
{
    return type_;
}

int Cell::getGold() const
{
    return gold_;
}

int Cell::getPlayer() const
{
    return player_;
}

void Cell::addBoat(int boatId)
{
    boatIds_.insert(boatId);
}

bool Cell::removeBoat(int boatId)
{
    return boatIds_.erase(boatId) != 0;
}

bool Cell::existsBoat(int boatId)
{
    return boatIds_.find(boatId) != boatIds_.end();
}
