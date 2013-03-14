#include "action-move.hh"
#include "constant.hh"
#include "game.hh"
#include "tools.hh"

ActionMove::ActionMove(int id_boat, position dest)
    : dest_(dest),
      id_boat_(id_boat)
{
}

int ActionMove::check(GameState* st) const
{
    if (!st->get_boats().count(id_boat_))
        return ID_INVALIDE;
    bateau boat = st->get_boats()[id_boat_];

    if (!boat.deplacable)
        return NON_DEPLACABLE;

    Cell* cell_o;
    if (!(cell_o = st->get_map()->get_cell(boat.pos)))
        return POSITION_INVALIDE;

    Cell* cell_d;
    if (!(cell_d = st->get_map()->get_cell(dest_)))
        return POSITION_INVALIDE;

    int max_move;
    if (boat.btype == BATEAU_CARAVELLE)
        max_move = CARAVELLE_DEPLACEMENT;
    else if (boat.btype == BATEAU_GALION)
        max_move = GALION_DEPLACEMENT;

    if (distance(boat.pos, dest_) > max_move)
        return TROP_LOIN;
    return OK;
}

void ActionMove::handle_buffer(utils::Buffer& buf)
{
    buf.handle(dest_);
    buf.handle(id_boat_);
}

void ActionMove::apply_on(GameState* st) const
{
    bateau boat = st->get_boats()[id_boat_];
    Cell* cell_d = st->get_map()->get_cell(dest_);
    Cell* cell_o = st->get_map()->get_cell(boat.pos);
    cell_o->remove_boat(id_boat_);
    cell_d->add_boat(id_boat_);
    boat.pos = dest_;
}
