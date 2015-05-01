#include <fstream>

#include <utils/log.hh>
#include <rules/player.hh>
#include <rules/actions.hh>

#include "rules.hh"
#include "game.hh"
#include "map.hh"
#include "api.hh"

#include "action-ack.hh"
#include "action-charge.hh"
#include "action-colonize.hh"
#include "action-construct.hh"
#include "action-discharge.hh"
#include "action-move.hh"
#include "action-transfer.hh"

Rules::Rules(const rules::Options opt)
  : TurnBasedRules(opt),
    sandbox_(opt.time)
{
    if (!opt.champion_lib.empty())
        champion_ = new utils::DLL(opt.champion_lib);
    else
        champion_ = nullptr;

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
    }

    api_->actions()->register_action(ID_ACTION_ACK,
            []() -> rules::IAction* { return new ActionAck(); });
    api_->actions()->register_action(ID_ACTION_CHARGE,
            []() -> rules::IAction* { return new ActionCharge(); });
    api_->actions()->register_action(ID_ACTION_COLONIZE,
            []() -> rules::IAction* { return new ActionColonize(); });
    api_->actions()->register_action(ID_ACTION_CONSTRUCT,
            []() -> rules::IAction* { return new ActionConstruct(); });
    api_->actions()->register_action(ID_ACTION_DISCHARGE,
            []() -> rules::IAction* { return new ActionDischarge(); });
    api_->actions()->register_action(ID_ACTION_MOVE,
            []() -> rules::IAction* { return new ActionMove(); });
    api_->actions()->register_action(ID_ACTION_TRANSFER,
            []() -> rules::IAction* { return new ActionTransfer(); });
}

Rules::Rules(rules::Players_sptr players, Api* api)
    : TurnBasedRules(rules::Options({"", "", 0, 0, rules::Player_sptr(),
                players, rules::Players_sptr(), 5})),
    api_(api),
    sandbox_()
{
}

Rules::~Rules()
{
    if (champion_)
        delete champion_;
    delete api_;
}

void Rules::at_start()
{
    api_->game_state()->increment_round();
}

void Rules::at_player_start()
{
    sandbox_.execute(champion_partie_init);
}

void Rules::at_spectator_start()
{
    champion_partie_init();
}

void Rules::at_player_end()
{
    sandbox_.execute(champion_partie_fin);
}

void Rules::at_spectator_end()
{
    champion_partie_fin();
}

bool Rules::is_finished()
{
    return api_->game_state()->is_finished();
}

rules::Actions* Rules::get_actions()
{
    return api_->actions();
}

void Rules::apply_action(const rules::IAction_sptr& action)
{
    api_->game_state_set(action->apply(api_->game_state()));
}

void Rules::player_turn()
{
    sandbox_.execute(champion_jouer_tour);
}

void Rules::spectator_turn()
{
    champion_jouer_tour();
    api_->actions()->add(
            rules::IAction_sptr(new ActionAck(api_->player()->id)));
}

void Rules::end_of_turn(uint32_t player_id)
{
    api_->game_state()->resolve_all_fights(player_id);
}

void Rules::end_of_round()
{
    api_->game_state()->resolve_all_scores();
    api_->game_state()->update_gold();
    api_->game_state()->update_boats();
    api_->game_state()->increment_round();
    api_->game_state()->clear_old_version();
}

void Rules::start_of_round()
{
    INFO("ROUND %d", api_->game_state()->get_current_round());
}
