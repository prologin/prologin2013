#ifndef ACTION_CHARGE_HH_
#define ACTION_CHARGE_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionCharge : public rules::Action<GameState>
{
    public:
        ActionCharge(int id_boat, int amount, int player);
        ActionCharge();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_CHARGE; }

    private:
        int id_boat_;
        int amount_;
        int player_id_;
};

#endif
