#ifndef ACTION_DISCHARGE_HH_
#define ACTION_DISCHARGE_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionDischarge : public rules::Action<GameState>
{
    public:
        ActionDischarge(int id_boat, int amount, int player);

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_DISCHARGE; }

    private:
        int id_boat_;
        int amount_;
        int player_id_;
};

#endif
