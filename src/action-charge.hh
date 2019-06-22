#ifndef ACTION_CHARGE_HH_
#define ACTION_CHARGE_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game.hh"

class ActionCharge final : public rules::Action<GameState>
{
public:
    ActionCharge(int id_boat, int amount, int player);
    ActionCharge();

    int check(const GameState& st) const override;
    void handle_buffer(utils::Buffer& buf) override;
    void apply_on(GameState* gameState) const override;

    uint32_t player_id() const override { return player_id_; }
    uint32_t id() const override { return ID_ACTION_CHARGE; }

private:
    int id_boat_;
    int amount_;
    int player_id_;
};

#endif
