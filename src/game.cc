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
    for (auto& p : players_->players)
        if (p->type == rules::PLAYER)
            player_ids_[p->id] = p;

    int i = 0;
    Cell* c;
    for (auto& p : player_ids_)
    {
        c = map->get_cell(map->get_start_position(i));
        if (!c)
            FATAL("starting position for player %d is invalid", i+1);
        if (c->get_type() != TERRAIN_ILE)
            FATAL("starting position for player %d is not an island", i+1);
        c->set_player(player_ids_[p.first]->id);
        i++;
    }
}

GameState::GameState(const GameState& st)
    : rules::GameState(st),
      map_(new Map(*st.map_)),
      players_(st.players_),
      current_turn_(st.current_turn_)
{
    boats_.insert(st.boats_.begin(), st.boats_.end());
    player_ids_.insert(st.player_ids_.begin(), st.player_ids_.end());
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

int GameState::get_score(int player_id) const
{
    if (player_ids_.find(player_id) == player_ids_.end())
        return -1;
    return player_ids_.find(player_id)->second->score;
}

int GameState::get_opponent(int player_id) const
{
    for (auto i : players_->players)
        if ((int) i->id != player_id)
            return i->id;
    return -1;
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
    return current_turn_ > FIN_PARTIE;
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

bateau* GameState::get_boat(int id)
{
    std::map<int, bateau>::iterator it = boats_.find(id);
    if (it == boats_.end())
      return NULL;
    return &(it->second);
}

void GameState::resolve_fight(position pos, int id_attacker)
{
    Cell* c = map_->get_cell(pos);
    std::set<int> boat_ids = c->get_id_boats();

    int id_defender = -1;

    int galions_attacker = 0;
    int galions_defender = 0;

    for (std::set<int>::iterator it = boat_ids.begin(); it != boat_ids.end(); ++it)
    {
        if (boats_[*it].joueur != id_attacker && id_defender == -1)
            id_defender = boats_[*it].joueur;

        if (boats_[*it].btype == BATEAU_GALION)
        {
            if (boats_[*it].joueur == id_attacker)
                galions_attacker++;
            if (boats_[*it].joueur == id_defender)
                galions_defender++;
        }
    }

    int galions_lost = std::min(galions_attacker, galions_defender) - 1;
    int id_winner = id_attacker;
    int id_loser = id_defender;

    if (galions_lost < 0)
        return;

    if (galions_attacker < galions_defender ||
        (galions_attacker == galions_defender && c->get_player() == id_defender))
        std::swap(id_winner, id_loser);

    if (c->get_player() != -1 && c->get_player() == id_loser)
        c->set_player(id_winner);

    int caravelle_winner = -1;
    for (std::set<int>::iterator i = boat_ids.begin(); i != boat_ids.end() && caravelle_winner == -1; ++i)
        if (boats_[*i].btype == BATEAU_CARAVELLE && boats_[*i].joueur == id_winner)
            caravelle_winner = boats_[*i].id;

    int gold_move = 0;
    for (std::set<int>::iterator it = boat_ids.begin(); it != boat_ids.end(); ++it)
    {
        if (boats_[*it].joueur == id_loser)
        {
            gold_move += boats_[*it].nb_or;
            boats_.erase(*it);
            c->remove_boat(*it);
        }
        else if (boats_[*it].btype == BATEAU_GALION && galions_lost > 0)
        {
            boats_.erase(*it);
            c->remove_boat(*it);
            galions_lost--;
        }
    }
    if (caravelle_winner != -1)
        boats_[caravelle_winner].nb_or += gold_move;
}

void GameState::resolve_all_fights(int id_attacker)
{
    for (int x = 0; x < TAILLE_TERRAIN; x++)
        for (int y = 0; y < TAILLE_TERRAIN; y++)
            resolve_fight({x, y}, id_attacker);
}

void GameState::resolve_score(position pos)
{
    Cell* c = map_->get_cell(pos);
    int owner = c->get_player();
    if (owner != -1)
        player_ids_[owner]->score += c->get_gold();
    for (auto& i : c->get_id_boats())
        player_ids_[boats_[i].joueur]->score += boats_[i].nb_or;
}

void GameState::resolve_all_scores()
{
    for (auto& i : players_->players)
        i->score = 0;
    for (int x = 0; x < TAILLE_TERRAIN; x++)
        for (int y = 0; y < TAILLE_TERRAIN; y++)
            resolve_score({x, y});
}

void GameState::update_gold()
{
    Cell* c;
    for (auto p : map_->get_islands())
    {
        c = map_->get_cell(p);
        if (c->get_player() != -1)
        {
            if (c->get_type() == TERRAIN_VOLCAN)
                c->set_gold(c->get_gold() + REVENU_VOLCAN);
            else if (c->get_type() == TERRAIN_ILE)
                c->set_gold(c->get_gold() + REVENU_ILE);
        }
    }
}
