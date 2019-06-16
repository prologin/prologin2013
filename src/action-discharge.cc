#include "action-discharge.hh"
#include "constant.hh"
#include "game.hh"

ActionDischarge::ActionDischarge(int id_boat, int amount, int player)
    : id_boat_(id_boat), amount_(amount), player_id_(player)
{}

ActionDischarge::ActionDischarge() : id_boat_(-1), amount_(0), player_id_(-1) {}

int ActionDischarge::check(const GameState* st) const
{
    Cell* island;
    const bateau* boat = const_cast<GameState*>(st)->get_boat(id_boat_);

    if (boat == NULL)
        return ID_INVALIDE;

    if (!(island = st->get_map()->get_cell(boat->pos)))
        return POSITION_INVALIDE;

    if (island->get_type() != TERRAIN_ILE &&
        island->get_type() != TERRAIN_VOLCAN)
        return ILE_INVALIDE;

    if (island->get_player() != player_id_)
        return ILE_ENNEMIE;

    if (boat->joueur != player_id_)
        return BATEAU_ENNEMI;

    if (amount_ < 0 || boat->nb_or < amount_)
        return OR_INSUFFISANT;

    return OK;
}

void ActionDischarge::handle_buffer(utils::Buffer& buf)
{
    buf.handle(id_boat_);
    buf.handle(amount_);
    buf.handle(player_id_);
}

void ActionDischarge::apply_on(GameState* st) const
{
    bateau* boat = st->get_boat(id_boat_);
    Cell* island = st->get_map()->get_cell(boat->pos);
    island->set_gold(island->get_gold() + amount_);
    boat->nb_or -= amount_;
}
