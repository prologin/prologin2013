#include <fstream>

#include <utils/log.hh>
#include <rules/player.hh>
#include <rules/actions.hh>

#include "rules.hh"
#include "game.hh"
#include "map.hh"
#include "api.hh"

#include "action-ack.hh"

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

    api_ = new Api(game_state, opt.player);

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
    CHECK(champion_ != nullptr);

    while (!api_->game_state()->is_finished())
    {
        INFO("TURN %d", api_->game_state()->get_current_turn());

        api_->actions()->clear();

        sandbox_.execute(champion_jouer_tour);

        uint32_t pulled_id;
        while (!msgr->wait_for_turn(api_->player()->id, &pulled_id))
            ;
        /* Send actions to the server */
        msgr->send_actions(*api_->actions());
        msgr->wait_for_ack();

        api_->actions()->clear();

        api_->game_state()->resolve_all_fights(api_->player()->id);

        /* Get other players actions */
        msgr->pull_actions(api_->actions());

        /* Apply them onto the gamestate */
        for (auto action : api_->actions()->actions())
        {
            api_->game_state_set(action->apply(api_->game_state()));
        }

        api_->game_state()->resolve_all_fights(api_->player()->id);
        api_->game_state()->resolve_all_scores();
    }
}

void Rules::spectator_loop(rules::ClientMessenger_sptr msgr)
{
    CHECK(champion_ != nullptr);

    while (!api_->game_state()->is_finished())
    {
        INFO("TURN %d", api_->game_state()->get_current_turn());

        api_->actions()->clear();

        champion_jouer_tour();

        uint32_t pulled_id;
        while (!msgr->wait_for_turn(api_->player()->id, &pulled_id))
            ;
        /* Send the ACK to the server (client can only send actions) */
        api_->actions()->add(
                rules::IAction_sptr(new ActionAck(api_->player()->id)));
        msgr->send_actions(*api_->actions());
        msgr->wait_for_ack();

        api_->actions()->clear();

        /* Get other players actions */
        msgr->pull_actions(api_->actions());

        /* Apply them onto the gamestate */
        for (auto action : api_->actions()->actions())
        {
            api_->game_state_set(action->apply(api_->game_state()));
        }

        api_->game_state()->resolve_all_fights(api_->player()->id);
        api_->game_state()->resolve_all_scores();
    }
}

void Rules::server_loop(rules::ServerMessenger_sptr msgr)
{
    CHECK(champion_ == nullptr);

    while (!api_->game_state()->is_finished())
    {
        INFO("TURN %d", api_->game_state()->get_current_turn());

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

            for (auto action : actions.actions())
            {
                api_->game_state_set(action->apply(api_->game_state()));
            }

            api_->game_state()->resolve_all_fights(api_->player()->id);

            msgr->push_actions(actions);
        }

        api_->game_state()->resolve_all_scores();

        for (unsigned int i = 0; i < spectators_->players.size(); i++)
        {
            msgr->push_id(spectators_->players[i]->id);
            rules::Actions actions;
            msgr->recv_actions(&actions);
            msgr->ack();
        }
    }
}
