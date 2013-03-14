#include "constant.hh"
#include <cstdlib>

void boat_kill(bateau &b)
{
    b.vivant = false;
    b.nb_or = 0;
    b.pos.x = -1;
    b.pos.y = -1;
}

int distance(position origin, position end)
{
    return abs(origin.x - end.x) + abs(origin.y - end.y);
}
