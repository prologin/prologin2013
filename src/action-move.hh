#ifndef ACTION_MOVE_HH_
#define ACTION_MOVE_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionMove : public rules::Action<GameState>
{
    public:
        ActionMove(int id, position dest);

        virtual int check(GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

    private:
        position dest_;
        int id_boat_;
};

#endif
