#ifndef ACTION_CONSTRUCT_HH_
#define ACTION_CONSTRUCT_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game.hh"

class ActionConstruct final : public rules::Action<GameState>
{
public:
    ActionConstruct(bateau_type btype, position pos, int player);
    ActionConstruct();

    int check(const GameState& st) const override;
    void handle_buffer(utils::Buffer& buf) override;
    void apply_on(GameState* gameState) const override;

    uint32_t player_id() const override { return player_id_; }
    uint32_t id() const override { return ID_ACTION_CONSTRUCT; }

private:
    bateau_type btype_;
    position pos_;
    int player_id_;
};

#endif
