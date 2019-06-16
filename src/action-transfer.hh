#ifndef ACTION_TRANSFER_HH_
#define ACTION_TRANSFER_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game.hh"

class ActionTransfer : public rules::Action<GameState>
{
public:
    ActionTransfer(int amount, int id_source, int id_dest, int player);
    ActionTransfer();

    int check(const GameState& st) const override;
    void handle_buffer(utils::Buffer& buf) override;
    void apply_on(GameState* gameState) const override;

    uint32_t player_id() const override { return player_id_; }
    uint32_t id() const override { return ID_ACTION_TRANSFER; }

private:
    int id_source_;
    int id_dest_;
    int amount_;
    int player_id_;
};

#endif
