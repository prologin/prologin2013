#ifndef GAME_HH_
# define GAME_HH_

# include <vector>

# include <rules/game-state.hh>
# include <rules/player.hh>

# include "map.hh"

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

    private:
        Map* map_;
        rules::Players_sptr players_;
        int current_turn_;
};

#endif // !GAME_HH_
