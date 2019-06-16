#ifndef ACTION_ACK_HH_

#define ACTION_ACK_HH_

#include <rules/action.hh>

#include "constant.hh"
#include "game.hh"

class ActionAck final : public rules::Action<GameState>
{
public:
    ActionAck(int player);

    ActionAck();

    int check(const GameState&) const override { return 0; }
    void handle_buffer(utils::Buffer&) override;

    uint32_t player_id() const override { return player_; }
    uint32_t id() const override { return ID_ACTION_ACK; }

protected:
    void apply_on(GameState*) const override {}

protected:
    int player_;
    int id_;
};

#endif /* !ACTION_ACK_HH_ */
