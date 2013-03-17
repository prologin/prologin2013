#include <cstdint>

#include <rules/player.hh>
#include <utils/log.hh>

#include "constant.hh"
#include "game.hh"
#include "map.hh"
#include "cell.hh"

GameState::GameState(Map* map, rules::Players_sptr players)
    : rules::GameState(),
      map_(map),
      players_(players),
      current_turn_(0),
      boat_next_id_(0)
{
}

GameState::GameState(const GameState& st)
    : rules::GameState(st),
      map_(new Map(*st.map_)),
      players_(st.players_),
      current_turn_(st.current_turn_)
{
    boats_.insert(st.boats_.begin(), st.boats_.end());
}

rules::GameState* GameState::copy() const
{
    return new GameState(*this);
}

GameState::~GameState()
{
    delete map_;
}

void GameState::init()
{
    DEBUG("GameState::init");
}

Map* GameState::get_map() const
{
    return map_;
}

size_t GameState::get_player_count() const
{
    return players_->players.size();
}

std::vector<int> GameState::get_scores() const
{
    std::vector<int> scores;
    for (auto it = players_->players.begin(); it != players_->players.end();
            ++it)
    {
        scores.push_back((*it)->score);
    }

    return scores;
}

int GameState::get_current_turn() const
{
    return current_turn_;
}

void GameState::increment_turn()
{
    current_turn_++;
}

bool GameState::is_finished()
{
    return current_turn_ == FIN_PARTIE;
}


bool GameState::add_boat(position origin, int player, bateau_type btype)
{
    int id = boat_next_id_++;
    if (boats_.count(id))
        return false;

    bateau boat;
    boat.id = id;
    boat.pos = origin;
    boat.joueur = player;
    boat.btype = btype;
    boat.nb_or = 0;
    boat.deplacable = false;
    boats_[id] = boat;

    map_->get_cell(origin)->add_boat(id);

    return true;
}

std::map<int, bateau> GameState::get_boats() const
{
    return boats_;
}

bateau* GameState::get_boat(int id)
{
    return &boats_[id];
}

void GameState::resolve_fight(position pos, int id_attacker)
{
    Cell* c = map_->get_cell(pos);
    std::set<int> boat_ids = c->get_id_boats();

    int id_p1 = -1;
    int id_p2 = -1;

    int galions_p1 = 0;
    int galions_p2 = 0;

    int galions_lost = 0;

    int caravelle_winner = -1;

    int id_winner = -1;
    int id_loser = -1;

    int i;

    for (std::set<int>::iterator it = c->get_id_boats().begin();
            it != c->get_id_boats().end(); it++)
    {
        i = *it;
        if (id_p1 == -1)
            id_p1 = boats_[i].joueur;
        else if (id_p2 == -1 && boats_[i].joueur != id_p1)
            id_p2 = boats_[i].joueur;

        if (boats_[i].btype == BATEAU_GALION)
        {
            if (boats_[i].joueur == id_p1)
                galions_p1++;
            if (boats_[i].joueur == id_p2)
                galions_p2++;
        }
    }


    if (!galions_p1 || !galions_p2)
        return;

    if (galions_p1 > galions_p2 ||
            (galions_p1 == galions_p2 &&
                ((c->get_player() == -1 && id_p1 == id_attacker) ||
                id_p1 == c->get_player())
            )
       )
    {
        id_winner = id_p1;
        id_loser = id_p2;
        galions_lost = galions_p2 - 1; /* The winner loses galions_enemy-1 */
    }
    else
    {
        id_winner = id_p2;
        id_loser = id_p1;
        galions_lost = galions_p1 - 1; /* The winner loses galions_enemy-1 */
    }

    if (c->get_player() == id_loser)
        c->set_player(id_winner);

    for (std::set<int>::iterator i = boat_ids.begin(); i != boat_ids.end();
            i++)
        if (boats_[*i].btype == BATEAU_CARAVELLE &&
            boats_[*i].joueur == id_winner)
            caravelle_winner = boats_[*i].id;

    int gold_move;

    for (std::set<int>::iterator it = boat_ids.begin(); it != boat_ids.end();
            it++)
    {
        i = *it;
        if (boats_[i].btype == BATEAU_CARAVELLE)
        {
            if (boats_[i].joueur != id_winner)
            {
                gold_move = boats_[i].nb_or;
                boats_.erase(i);
                c->remove_boat(i);
                if (caravelle_winner != -1)
                    boats_[caravelle_winner].nb_or += gold_move;
            }
        }
        else
        {
            if (boats_[i].joueur == id_winner && galions_lost)
            {
                boats_.erase(i);
                c->remove_boat(i);
                galions_lost--;
            }
            else if (boats_[i].joueur == id_loser)
            {
                boats_.erase(i);
                c->remove_boat(i);
            }
        }
    }
}
