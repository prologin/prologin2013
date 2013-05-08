#include "action-transfer.hh"
#include "constant.hh"
#include "game.hh"

ActionTransfer::ActionTransfer(int amount, int id_source, int id_dest,
        int player)
    : id_source_(id_source),
      id_dest_(id_dest),
      amount_(amount),
      player_id_(player)
{
}

ActionTransfer::ActionTransfer()
    : id_source_(-1),
      id_dest_(-1),
      amount_(0),
      player_id_(-1)
{
}

int ActionTransfer::check(const GameState* st) const
{
    const bateau* source = const_cast<GameState*>(st)->get_boat(id_source_);
    const bateau* dest = const_cast<GameState*>(st)->get_boat(id_dest_);

    if (!(source && dest))
        return ID_INVALIDE;

    if (source->pos != dest->pos)
        return POSITION_INVALIDE;

    if (source->joueur != player_id_ || dest->joueur != player_id_)
        return BATEAU_ENNEMI;

    if (amount_ < 0 || source->nb_or < amount_)
        return OR_INSUFFISANT;

    return OK;
}

void ActionTransfer::handle_buffer(utils::Buffer& buf)
{
    buf.handle(id_source_);
    buf.handle(id_dest_);
    buf.handle(amount_);
    buf.handle(player_id_);
}

void ActionTransfer::apply_on(GameState* st) const
{
    bateau* source = st->get_boat(id_source_);
    bateau* dest = st->get_boat(id_dest_);
    source->nb_or -= amount_;
    dest->nb_or += amount_;
}
