#ifndef ACTION_MOVE_HH_
#define ACTION_MOVE_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionMove : public rules::Action<GameState>
{
    public:
        ActionMove(int id, position dest, int player);
        ActionMove();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_MOVE; }

    private:
        position dest_;
        int id_boat_;
        int player_id_;
};

#endif
