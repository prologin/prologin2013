#ifndef GAME_HH_
#define GAME_HH_

#include <vector>
#include <map>

#include <rules/game-state.hh>
#include <rules/player.hh>

#include "constant.hh"
#include "map.hh"

enum action_id
{
    ID_ACTION_ACK,
    ID_ACTION_CHARGE,
    ID_ACTION_COLONIZE,
    ID_ACTION_CONSTRUCT,
    ID_ACTION_DISCHARGE,
    ID_ACTION_MOVE,
    ID_ACTION_TRANSFER,
};


class GameState : public rules::GameState
{
    public:
        GameState(Map* map, rules::Players_sptr players);
        GameState(const GameState& st);
        virtual rules::GameState* copy() const;
        ~GameState();

        void init();

        Map* get_map() const;

        int get_score(int player) const;

        int get_opponent(int player) const;

        int get_current_turn() const;
        void increment_turn();

        bool is_finished();

        bool add_boat(position origin, int player, bateau_type btype);
        bateau* get_boat(int id);

        int get_last_id() const;
        int get_nb_boats(int player_id) const;

        void resolve_fight(position pos, int id_attacker);
        void resolve_all_fights(int id_attacker);

        void resolve_score(position pos);
        void resolve_all_scores();

        void update_gold();
        void update_boats();

        /* Accessors used by the dumper. */
        rules::Players_sptr get_players() const
        { return players_; }

        const std::map<int, bateau>& get_boats() const
        { return boats_; }
    private:
        Map* map_;
        rules::Players_sptr players_;
        std::map<int, rules::Player_sptr> player_ids_;
        int current_turn_;

        int boat_next_id_;
        std::map<int, bateau> boats_;
        std::map<int, int> nb_boats_;
};

#endif // !GAME_HH_
