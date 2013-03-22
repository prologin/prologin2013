#ifndef ACTION_CONSTRUCT_HH_
#define ACTION_CONSTRUCT_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionConstruct : public rules::Action<GameState>
{
    public:
        ActionConstruct(bateau_type btype, position pos, int player);
        ActionConstruct();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_CONSTRUCT; }

    private:
        bateau_type btype_;
        position pos_;
        int player_id_;
};

#endif
