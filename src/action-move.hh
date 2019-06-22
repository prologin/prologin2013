#ifndef ACTION_MOVE_HH_
#define ACTION_MOVE_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game.hh"

class ActionMove : public rules::Action<GameState>
{
public:
    ActionMove(int id, position dest, int player);
    ActionMove();

    int check(const GameState& st) const override;
    void handle_buffer(utils::Buffer& buf) override;
    void apply_on(GameState* gameState) const override;

    uint32_t player_id() const override { return player_id_; }
    uint32_t id() const override { return ID_ACTION_MOVE; }

private:
    position dest_;
    int id_boat_;
    int player_id_;
};

#endif
