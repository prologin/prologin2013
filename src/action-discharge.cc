#include "action-discharge.hh"
#include "constant.hh"
#include "game.hh"

ActionDischarge::ActionDischarge(int id_boat, position pos, int amount,
        int player)
    : id_boat_(id_boat),
      pos_(pos),
      amount_(amount),
      player_id_(player)
{
}

int ActionDischarge::check(const GameState* st) const
{
    Cell* island;
    if (!(island = st->get_map()->get_cell(pos_)))
        return POSITION_INVALIDE;

    if (island->get_type() != TERRAIN_ILE &&
        island->get_type() != TERRAIN_VOLCAN)
        return ILE_INVALIDE;

    if (island->get_player() != player_id_)
        return ILE_ENNEMIE;

    if (!st->get_boats().count(id_boat_))
        return ID_INVALIDE;

    bateau boat = st->get_boats()[id_boat_];

    if (boat.joueur != player_id_)
        return BATEAU_ENNEMI;

    if (boat.pos != pos_)
        return POSITION_INVALIDE;

    if (boat.nb_or < amount_)
        return OR_INSUFFISANT;

    return OK;
}

void ActionDischarge::handle_buffer(utils::Buffer& buf)
{
    buf.handle(id_boat_);
    buf.handle(pos_);
    buf.handle(amount_);
    buf.handle(player_id_);
}

void ActionDischarge::apply_on(GameState* st) const
{
    Cell* island = st->get_map()->get_cell(pos_);
    island->set_gold(island->get_gold() + amount_);
    st->get_boat(id_boat_)->nb_or -= amount_;
}
