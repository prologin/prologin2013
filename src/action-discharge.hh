#ifndef ACTION_DISCHARGE_HH_
#define ACTION_DISCHARGE_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionDischarge : public rules::Action<GameState>
{
    public:
        ActionDischarge(int id_boat, position pos, int amount, int player);

        virtual int check(GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

    private:
        int id_boat_;
        position pos_;
        int amount_;
        int player_id_;
};

#endif
