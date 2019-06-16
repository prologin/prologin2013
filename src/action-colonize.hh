#ifndef ACTION_COLONIZE_HH_
#define ACTION_COLONIZE_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game.hh"

class ActionColonize : public rules::Action<GameState>
{
public:
    ActionColonize(position pos, int player);
    ActionColonize();

    virtual int check(const GameState* st) const;
    virtual void handle_buffer(utils::Buffer& buf);
    virtual void apply_on(GameState* gameState) const;

    uint32_t player_id() const { return player_id_; }
    uint32_t id() const { return ID_ACTION_COLONIZE; }

private:
    bateau_type btype_;
    position pos_;
    int player_id_;
};

#endif
