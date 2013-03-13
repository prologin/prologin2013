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
      current_turn_(0)
{
}

GameState::GameState(const GameState& st)
    : rules::GameState(st),
      map_(new Map(*st.map_)),
      players_(st.players_),
      current_turn_(st.current_turn_)
{
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
    for (auto it = players_->players.begin(); it != players_->players.end(); ++it)
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
    current_turn_ += 1;
}

bool GameState::is_finished()
{
    return current_turn_ == FIN_PARTIE;
}
