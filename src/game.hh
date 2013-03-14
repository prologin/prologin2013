#ifndef GAME_HH_
#define GAME_HH_

#include <vector>
#include <map>

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "constant.hh"
#include "map.hh"

class GameState : public rules::GameState
{
    public:
        GameState(Map* map, rules::Players_sptr players);
        GameState(const GameState& st);
        virtual rules::GameState* copy() const;
        ~GameState();

        void init();

        Map* get_map() const;

        size_t get_player_count() const;

        std::vector<int> get_scores() const;

        int get_current_turn() const;
        void increment_turn();

        bool is_finished();


        bool add_boat(position origin, int player, bateau_type btype);
        std::map<int, bateau> get_boats();

    private:
        Map* map_;
        rules::Players_sptr players_;
        int current_turn_;

        int boat_next_id_;
        std::map<int, bateau> boats_;
};

#endif // !GAME_HH_
