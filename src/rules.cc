#include <fstream>

#include <utils/log.hh>
#include <rules/player.hh>

#include "rules.hh"
#include "game.hh"
#include "map.hh"
#include "api.hh"

Rules::Rules(const rules::Options opt)
    : opt_(opt)
{
    if (!opt.champion_lib.empty())
        champion_dll_ = new utils::DLL(opt.champion_lib);

    std::ifstream ifs(opt.map_file);
    Map* map = new Map;
    map->load(ifs);

    GameState* game_state = new GameState(map, opt.players);
    game_state->init();

    int equipe = 0;
    if (opt.player.get() != nullptr)
    {
        for (; (unsigned)equipe < opt.players->players.size(); ++equipe)
            if (opt.players->players[equipe]->id == opt.player->id)
                break;
    }

    api_ = new Api(game_state, opt.player, equipe);

    // Get the champion library if we are a client
    if (!opt.champion_lib.empty())
    {
        champion_ = new utils::DLL(opt.champion_lib);
        champion_partie_init = champion_->get<f_champion_partie_init>("partie_init");
        champion_jouer_placement = champion_->get<f_champion_jouer_placement>("jouer_placement");
        champion_jouer_deplacement = champion_->get<f_champion_jouer_deplacement>("jouer_deplacement");
        champion_jouer_attaque = champion_->get<f_champion_jouer_attaque>("jouer_attaque");
        champion_partie_fin = champion_->get<f_champion_partie_fin>("partie_fin");

        if (opt.player->type == rules::SPECTATOR)
            champion_partie_init();
        else
            sandbox_.execute(champion_partie_init);
    }

    players_ = opt.players;
    spectators_ = opt.spectators;
    timeout_ = opt.time;
}

Rules::~Rules()
{
    // FIXME

    delete champion_dll_;
}

void Rules::client_loop(rules::ClientMessenger_sptr msgr)
{
    // FIXME
}

void Rules::spectator_loop(rules::ClientMessenger_sptr msgr)
{
    // FIXME
}

void Rules::server_loop(rules::ServerMessenger_sptr msgr)
{
    // FIXME
}
