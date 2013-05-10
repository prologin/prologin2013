#ifndef ACTION_TRANSFER_HH_
#define ACTION_TRANSFER_HH_

#include <rules/action.hh>

#include "game.hh"
#include "constant.hh"

class ActionTransfer : public rules::Action<GameState>
{
    public:
        ActionTransfer(int amount, int id_source, int id_dest, int player);
        ActionTransfer();

        virtual int check(const GameState* st) const;
        virtual void handle_buffer(utils::Buffer& buf);
        virtual void apply_on(GameState* gameState) const;

        uint32_t player_id() const { return player_id_; }
        uint32_t id() const { return ID_ACTION_TRANSFER; }

    private:
        int id_source_;
        int id_dest_;
        int amount_;
        int player_id_;
};

#endif
