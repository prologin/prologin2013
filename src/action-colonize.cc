#include "action-colonize.hh"
#include "constant.hh"
#include "game.hh"
#include "tools.hh"

ActionColonize::ActionColonize(position pos, int player)
    : pos_(pos), player_id_(player)
{}

ActionColonize::ActionColonize() : pos_({-1, -1}), player_id_(-1) {}

int ActionColonize::check(const GameState* st) const
{
    Cell* island;
    if (!(island = st->get_map()->get_cell(pos_)))
        return POSITION_INVALIDE;

    if (island->get_type() != TERRAIN_ILE &&
        island->get_type() != TERRAIN_VOLCAN)
        return ILE_INVALIDE;
    if (island->get_player() != -1)
        return ILE_COLONISEE;

    std::set<int> list_boats = island->get_id_boats();
    for (std::set<int>::iterator it = list_boats.begin();
         it != list_boats.end(); it++)
    {
        bateau* boat = const_cast<GameState*>(st)->get_boat(*it);
        if (boat->btype == BATEAU_CARAVELLE && boat->joueur == player_id_)
            return OK;
    }

    return AUCUNE_CARAVELLE;
}

void ActionColonize::handle_buffer(utils::Buffer& buf)
{
    buf.handle(btype_);
    buf.handle(pos_);
    buf.handle(player_id_);
}

void ActionColonize::apply_on(GameState* st) const
{
    st->get_map()->get_cell(pos_)->set_player(player_id_);
}
