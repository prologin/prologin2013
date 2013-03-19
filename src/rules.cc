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

bool Rules::is_spectator(uint32_t id)
{
    for (rules::Player_sptr spectator : spectators_->players)
        if (spectator->id == id)
            return true;
    return false;
}

void Rules::end_of_move(uint32_t player_id)
{
    api_->game_state()->resolve_all_fights(player_id);
}

void Rules::end_of_turn()
{
    api_->game_state()->resolve_all_scores();
    api_->game_state()->update_gold();
    api_->game_state()->increment_turn();
    INFO("TURN %d", api_->game_state()->get_current_turn());
}

void Rules::client_loop(rules::ClientMessenger_sptr msgr)
{
    CHECK(champion_ != nullptr);

    INFO("TURN %d", api_->game_state()->get_current_turn());
    while (!api_->game_state()->is_finished())
    {
        /* We first retrieve the ID of the first player, then we check for
         * each player if he is the first to play. If so, we executes
         * end_of_turn() before doing anything. Then, we play the move and call
         * end_of_move().
         */

        uint32_t playing_id = -1;
        int first_player;

        /* Other players turns */
        while (!api_->game_state()->is_finished() &&
               msgr->wait_for_turn(opt_.player->id, &playing_id))
        {

            /* End of each turn */
            if (first_player == (int) playing_id)
                end_of_turn();

            if (first_player == -1)
                first_player = playing_id;

            if (api_->game_state()->is_finished())
                break;

            /* Pass if spectator */
            if (is_spectator(playing_id))
                continue;

            /* Clear the list of actions*/
            api_->actions()->clear();

            /* Get current player actions */
            msgr->pull_actions(api_->actions());

            /* Apply them onto the gamestate */
            for (auto action : api_->actions()->actions())
                if (action->player_id() != api_->player()->id)
                    api_->game_state_set(action->apply(api_->game_state()));
            msgr->wait_for_ack();

            /* End of each move */
            end_of_move(playing_id);
        }

        /* End of each turn */
        if (first_player == (int) playing_id)
            end_of_turn();

        if (first_player == -1)
            first_player = playing_id;

        if (api_->game_state()->is_finished())
            break;

        api_->actions()->clear();
        sandbox_.execute(champion_jouer_tour);
        msgr->send_actions(*api_->actions());
        msgr->wait_for_ack();
        msgr->pull_actions(api_->actions());
        api_->actions()->clear();

        end_of_move(api_->player()->id);
    }
}

void Rules::spectator_loop(rules::ClientMessenger_sptr msgr)
{
    CHECK(champion_ != nullptr);

    INFO("TURN %d", api_->game_state()->get_current_turn());
    while (!api_->game_state()->is_finished())
    {
        api_->actions()->clear();

        champion_jouer_tour();

        uint32_t pulled_id;
        while (!msgr->wait_for_turn(api_->player()->id, &pulled_id))
            end_of_move(pulled_id);

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
            api_->game_state_set(action->apply(api_->game_state()));

        end_of_turn();
    }
}

void Rules::server_loop(rules::ServerMessenger_sptr msgr)
{
    CHECK(champion_ == nullptr);

    INFO("TURN %d", api_->game_state()->get_current_turn());
    while (!api_->game_state()->is_finished())
    {
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
                api_->game_state_set(action->apply(api_->game_state()));

            msgr->push_actions(actions);
            end_of_move(i);
        }

        for (unsigned int i = 0; i < spectators_->players.size(); i++)
        {
            msgr->push_id(spectators_->players[i]->id);
            rules::Actions actions;
            msgr->recv_actions(&actions);
            msgr->ack();
        }

        end_of_turn();
    }
}
