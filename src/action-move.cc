#include "action-move.hh"
#include "constant.hh"
#include "game.hh"
#include "tools.hh"

ActionMove::ActionMove(int id_boat, position dest, int player_id)
    : dest_(dest),
      id_boat_(id_boat),
      player_id_(player_id)
{
}

int ActionMove::check(GameState* st) const
{
    if (!st->get_boats().count(id_boat_))
        return ID_INVALIDE;
    bateau boat = st->get_boats()[id_boat_];

    Cell* cell_o;
    if (!(cell_o = st->get_map()->get_cell(boat.pos)))
        return POSITION_INVALIDE;

    int max_move;
    if (boat.btype == BATEAU_CARAVELLE)
        max_move = CARAVELLE_DEPLACEMENT;
    else if (boat.btype == BATEAU_GALION)
        max_move = GALION_DEPLACEMENT;

    if (distance(boat.pos, dest_) <= max_move)
        return OK;
}
