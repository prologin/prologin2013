#include <constant.hh>

void boat_kill(bateau &b)
{
    b.vivant = false;
    b.nb_or = 0;
    b.pos.x = -1;
    b.pos.y = -1;
}
