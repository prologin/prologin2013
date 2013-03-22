#include "action-construct.hh"
#include "constant.hh"
#include "game.hh"
#include "tools.hh"

ActionConstruct::ActionConstruct(bateau_type btype, position pos, int player)
    : btype_(btype),
      pos_(pos),
      player_id_(player)
{
}

int ActionConstruct::check(const GameState* st) const
{
    Cell* island;
    if (!(island = st->get_map()->get_cell(pos_)))
        return POSITION_INVALIDE;
    if (island->get_type() != TERRAIN_ILE)
        return ILE_INVALIDE;

    if (island->get_player() != player_id_)
        return ILE_ENNEMIE;

    int cost = (btype_ == BATEAU_GALION) ? GALION_COUT : CARAVELLE_COUT;
    if (island->get_gold() < cost || btype_ == BATEAU_ERREUR)
        return OR_INSUFFISANT;

    return OK;
}

void ActionConstruct::handle_buffer(utils::Buffer& buf)
{
    buf.handle(btype_);
    buf.handle(pos_);
    buf.handle(player_id_);
}

void ActionConstruct::apply_on(GameState* st) const
{
    int cost = (btype_ == BATEAU_GALION) ? GALION_COUT : CARAVELLE_COUT;
    Cell* island = st->get_map()->get_cell(pos_);
    island->set_gold(island->get_gold() - cost);

    st->add_boat(pos_, player_id_, btype_);
}
