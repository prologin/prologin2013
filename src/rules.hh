#ifndef RULES_RULES_HH_
#define RULES_RULES_HH_

#include <utils/dll.hh>
#include <utils/sandbox.hh>
#include <rules/options.hh>
#include <rules/client-messenger.hh>
#include <rules/server-messenger.hh>
#include <rules/player.hh>

#include "api.hh"

typedef void (*f_champion_partie_init)();
typedef void (*f_champion_jouer_tour)();
typedef void (*f_champion_partie_fin)();

class Rules
{
    public:
        explicit Rules(const rules::Options opt);
        virtual ~Rules();

        // FIXME
        void client_loop(rules::ClientMessenger_sptr msgr);
        void spectator_loop(rules::ClientMessenger_sptr msgr);
        void server_loop(rules::ServerMessenger_sptr msgr);

    protected:
        f_champion_partie_init champion_partie_init;
        f_champion_jouer_tour champion_jouer_tour;
        f_champion_partie_fin champion_partie_fin;
    private:
        const rules::Options opt_;

        utils::DLL* champion_;
        Api* api_;

        rules::Players_sptr players_;
        rules::Players_sptr spectators_;

        int timeout_;
        
        utils::Sandbox sandbox_;
};

#endif // !RULES_RULES_HH_
