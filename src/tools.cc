#include "constant.hh"
#include <cstdlib>

int distance(position origin, position end)
{
    return abs(origin.x - end.x) + abs(origin.y - end.y);
}
