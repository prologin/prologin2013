#ifndef RULES_RULES_HH_
#define RULES_RULES_HH_

#include <utils/dll.hh>
#include <utils/sandbox.hh>
#include <rules/options.hh>
#include <rules/rules.hh>

#include "api.hh"

typedef void (*f_champion_partie_init)();
typedef void (*f_champion_jouer_tour)();
typedef void (*f_champion_partie_fin)();

class Rules : public rules::TurnBasedRules
{
    public:
        explicit Rules(const rules::Options opt);
        Rules(rules::Players_sptr players, Api* api);
        virtual ~Rules();

        // Function executed at the start of the game
        virtual void at_start();
        virtual void at_client_start();
        virtual void at_spectator_start();

        // Functions executed at the end of the game
        virtual void at_client_end();
        virtual void at_spectator_end();

        // Check whether the game is over
        virtual bool is_finished();

        // Get the actions structure with registered actions
        virtual rules::Actions* get_actions();

        // Apply an action to the game state
        virtual void apply_action(const rules::IAction_sptr& action);

        // Player's turn: call the champion within a sandbox
        virtual void player_turn();

        // Spectator's turn: call the champion wihout any sandbox
        virtual void spectator_turn();

        // Called each time a player has finished its turn
        virtual void end_of_turn(uint32_t);

        // Called before/after every player has played
        virtual void start_of_round();
        virtual void end_of_round();

    protected:
        f_champion_partie_init champion_partie_init;
        f_champion_jouer_tour champion_jouer_tour;
        f_champion_partie_fin champion_partie_fin;

    private:
        utils::DLL* champion_;
        Api* api_;

        utils::Sandbox sandbox_;
};

#endif // !RULES_RULES_HH_
