#ifndef ACTION_COLONIZE_HH_
#define ACTION_COLONIZE_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionColonize : public rules::Action<GameState>
{
    public:
        ActionColonize(position pos, int player);

        virtual int check(GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

    private:
        bateau_type btype_;
        position pos_;
        int player_id_;
};

#endif
