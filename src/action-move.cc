#include "action-move.hh"
#include "constant.hh"
#include "game.hh"
#include "tools.hh"

ActionMove::ActionMove(int id_boat, position dest, int player)
    : dest_(dest),
      id_boat_(id_boat),
      player_id_(player)
{
}

ActionMove::ActionMove()
    : dest_({-1, -1}),
      id_boat_(-1),
      player_id_(-1)
{
}

int ActionMove::check(const GameState* st) const
{
    const bateau* boat = const_cast<GameState*>(st)->get_boat(id_boat_);

    if (boat == NULL)
        return ID_INVALIDE;

    if (boat->joueur != player_id_)
        return BATEAU_ENNEMI;

    if (!boat->deplacable)
        return NON_DEPLACABLE;

    Cell* cell_o;
    if (!(cell_o = st->get_map()->get_cell(boat->pos)))
        return POSITION_INVALIDE;

    Cell* cell_d;
    if (!(cell_d = st->get_map()->get_cell(dest_)))
        return POSITION_INVALIDE;

    int max_move = 0;
    if (boat->btype == BATEAU_CARAVELLE)
        max_move = CARAVELLE_DEPLACEMENT;
    else if (boat->btype == BATEAU_GALION)
        max_move = GALION_DEPLACEMENT;

    if (distance(boat->pos, dest_) > max_move)
        return TROP_LOIN;
    return OK;
}

void ActionMove::handle_buffer(utils::Buffer& buf)
{
    buf.handle(dest_);
    buf.handle(id_boat_);
    buf.handle(player_id_);
}

void ActionMove::apply_on(GameState* st) const
{
    Cell* dest = st->get_map()->get_cell(dest_);
    bateau* boat = st->get_boat(id_boat_);
    dest->add_boat(id_boat_);
    st->get_map()->get_cell(boat->pos)->remove_boat(id_boat_);
    boat->pos = dest_;
}
