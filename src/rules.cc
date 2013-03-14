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
        champion_ = new utils::DLL(opt.champion_lib);

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
        champion_partie_init =
            champion_->get<f_champion_partie_init>("partie_init");
        champion_jouer_tour =
            champion_->get<f_champion_jouer_tour>("jouer_tour");
        champion_partie_fin =
            champion_->get<f_champion_partie_fin>("partie_fin");

        if (opt.player->type == rules::SPECTATOR)
            champion_partie_init();
        else
            sandbox_.execute(champion_partie_init);
    }

    players_ = opt.players;
    spectators_ = opt.spectators;
    timeout_ = opt.time;
}

Rules::Rules(rules::Players_sptr players, Api* api)
    : champion_(nullptr),
      api_(api),
      players_(players),
      sandbox_()
{
}

Rules::~Rules()
{
    if (champion_)
    {
        if (opt_.player->type == rules::SPECTATOR)
            champion_partie_fin();
        else
            sandbox_.execute(champion_partie_fin);

        delete champion_;
    }

    delete api_;
}

void Rules::client_loop(rules::ClientMessenger_sptr msgr)
{
#if 0
    CHECK(champion_ != nullptr);

    while (!is_finished())
    {
        INFO("TURN %d", api_->game_state()->getCurrentTurn());

        api_->actions()->clear();

        sandbox_.execute(champion_jouer_tour);

        uint32_t pulled_id;
        while (!msgr->wait_for_turn(api_->player()->id, &pulled_id))
            ;
        /* Send actions to the server */
        msgr->send_actions(*api_->actions());
        msgr->wait_for_ack();

        api_->actions()->clear();

        resolve_fights();

        /* Get other players actions */
        msgr->pull_actions(api_->actions());

        /* Apply them onto the gamestate */
        for (IAction_sptr action : api_->actions()->actions())
        {
            api_->game_state_set(action->apply(api_->game_state()));
        }

        resolve_fights();
        resolve_points();
    }
#endif
}

void Rules::spectator_loop(rules::ClientMessenger_sptr msgr)
{
#if 0
    CHECK(champion_ != nullptr);

    while (!is_finished())
    {
        INFO("TURN %d", api_->game_state()->getCurrentTurn());

        api_->actions()->clear();

        champion_jouer_tour();

        uint32_t pulled_id;
        while (!msgr->wait_for_turn(api_->player()->id, &pulled_id))
            ;
        /* Send the ACK to the server (client can only send actions) */
        api_actions->add(rules::IAction_sptr(new ActionAck(api_->player()->id)));
        msgr->send_actions(*api_->actions());
        msgr->wait_for_ack();

        api_->actions()->clear();

        /* Get other players actions */
        msgr->pull_actions(api_->actions());

        /* Apply them onto the gamestate */
        for (IAction_sptr action : api_->actions()->actions())
        {
            api_->game_state_set(action->apply(api_->game_state()));
        }

        resolve_fights();
        resolve_points();
    }
#endif
}

void Rules::server_loop(rules::ServerMessenger_sptr msgr)
{
#if 0
    CHECK(champion_ == nullptr);

    while (!is_finished())
    {
        INFO("TURN %d", api_->game_state()->getCurrentTurn());

        for (unsigned int i = 0; i < players_->players.size(); i++)
        {
            msgr->push_id(players_->players[i]->id);
            if (!msgr->poll(timeout_))
            {
                continue;
            }

            rules::Actions actions;
            msgr->recv_actions(&actions);
            msgr->ack();

            for (IAction_sptr action : actions)
            {
                api_->game_state_set(action->apply(api_->game_state()));
            }
            resolve_fights();

            msgr->push_actions(actions);
        }
        resolve_score();

        for (unsigned int i = 0; i < spectators_->players.size(); i++)
        {
            msgr->push_id(spectators_->players[i]->id);
            rules::Actions actions;
            msgr->recv_actions(&actions);
            msgr->ack();
        }
    }
#endif
}
