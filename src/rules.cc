#include <cstdlib>
#include <fstream>

#include <rules/actions.hh>
#include <rules/player.hh>
#include <utils/log.hh>

#include "api.hh"
#include "game.hh"
#include "map.hh"
#include "rules.hh"

#include "action-ack.hh"
#include "action-charge.hh"
#include "action-colonize.hh"
#include "action-construct.hh"
#include "action-discharge.hh"
#include "action-move.hh"
#include "action-transfer.hh"

Rules::Rules(const rules::Options opt) : TurnBasedRules(opt), sandbox_(opt.time)
{
    std::ifstream ifs(opt.map_file);
    Map* map = new Map;
    map->load(ifs);

    auto game_state = std::make_unique<GameState>(map, opt.players);
    game_state->init();

    api_ = std::make_unique<Api>(std::move(game_state), opt.player);

    // Get the champion library if we are a client
    if (!opt.champion_lib.empty())
    {
        champion_ = std::make_unique<utils::DLL>(opt.champion_lib);
        champion_partie_init =
            champion_->get<f_champion_partie_init>("partie_init");
        champion_jouer_tour =
            champion_->get<f_champion_jouer_tour>("jouer_tour");
        champion_partie_fin =
            champion_->get<f_champion_partie_fin>("partie_fin");
    }

    api_->actions()->register_action(
        ID_ACTION_ACK, []() { return std::make_unique<ActionAck>(); });
    api_->actions()->register_action(
        ID_ACTION_CHARGE, []() { return std::make_unique<ActionCharge>(); });
    api_->actions()->register_action(ID_ACTION_COLONIZE, []() {
        return std::make_unique<ActionColonize>();
    });
    api_->actions()->register_action(ID_ACTION_CONSTRUCT, []() {
        return std::make_unique<ActionConstruct>();
    });
    api_->actions()->register_action(ID_ACTION_DISCHARGE, []() {
        return std::make_unique<ActionDischarge>();
    });
    api_->actions()->register_action(
        ID_ACTION_MOVE, []() { return std::make_unique<ActionMove>(); });
    api_->actions()->register_action(ID_ACTION_TRANSFER, []() {
        return std::make_unique<ActionTransfer>();
    });
}

void Rules::at_start()
{
    api_->game_state().increment_round();
}

void Rules::at_player_start(rules::ClientMessenger_sptr)
{
    sandbox_.execute(champion_partie_init);
}

void Rules::at_spectator_start(rules::ClientMessenger_sptr)
{
    champion_partie_init();
}

void Rules::at_player_end(rules::ClientMessenger_sptr)
{
    sandbox_.execute(champion_partie_fin);
}

void Rules::at_spectator_end(rules::ClientMessenger_sptr)
{
    champion_partie_fin();
}

bool Rules::is_finished()
{
    return api_->game_state().is_finished();
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction& action)
{
    // When receiving an action, the API should have already checked that it
    // is valid. We recheck that for the current gamestate here to avoid weird
    // client/server desynchronizations and make sure the gamestate is always
    // consistent across the clients and the server.
    int err = api_->game_state_check(action);
    if (err)
        FATAL("Synchronization error: received action %d from player %d, but "
              "check() on current gamestate returned %d.",
              action.id(), action.player_id(), err);

    api_->game_state_apply(action);
}

void Rules::player_turn()
{
    sandbox_.execute(champion_jouer_tour);
}

void Rules::spectator_turn()
{
    champion_jouer_tour();
    api_->actions()->add(std::make_unique<ActionAck>(api_->player()->id));
}

void Rules::end_of_player_turn(uint32_t player_id)
{
    api_->game_state().resolve_all_fights(player_id);
}

void Rules::end_of_round()
{
    api_->game_state().resolve_all_scores();
    api_->game_state().update_gold();
    api_->game_state().update_boats();
    api_->game_state().increment_round();
    api_->clear_old_game_states();
}

void Rules::start_of_round()
{
    INFO("ROUND %d", api_->game_state().get_current_round());
}

void Rules::dump_state(std::ostream& out)
{
    char* line = api_->get_dump();
    out << line << std::endl;
    free(line);
}
