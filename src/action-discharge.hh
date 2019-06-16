#ifndef ACTION_DISCHARGE_HH_
#define ACTION_DISCHARGE_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game.hh"

class ActionDischarge final : public rules::Action<GameState>
{
public:
    ActionDischarge(int id_boat, int amount, int player);
    ActionDischarge();

    int check(const GameState& st) const override;
    void handle_buffer(utils::Buffer& buf) override;
    void apply_on(GameState* gameState) const override;

    uint32_t player_id() const override { return player_id_; }
    uint32_t id() const override { return ID_ACTION_DISCHARGE; }

private:
    int id_boat_;
    int amount_;
    int player_id_;
};

#endif
