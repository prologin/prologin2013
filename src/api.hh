/*
 ** Stechec project is free software; you can redistribute it and/or modify
 ** it under the terms of the GNU General Public License as published by
 ** the Free Software Foundation; either version 2 of the License, or
 ** (at your option) any later version.
 **
 ** The complete GNU General Public Licence Notice can be found as the
 ** `NOTICE' file in the root directory.
 **
 ** Copyright (C) 2013 Prologin
 */

#ifndef API_HH_
#define API_HH_

#include <vector>
#include <rules/game-state.hh>
#include <rules/player.hh>
#include <rules/actions.hh>

#include "constant.hh"
#include "game.hh"

/*!
 ** Method of this call are called by the candidat, throught 'interface.cc'
 */
class Api
{

    public:
        Api(GameState* game_state, rules::Player_sptr player);
        virtual ~Api() { }

        const rules::Player_sptr player() const { return player_; }
        void player_set(rules::Player_sptr player) { player_ = player; }
        
        rules::Actions* actions() { return &actions_; }

        const GameState* game_state() const { return game_state_; }
        GameState* game_state() { return game_state_; }
        void game_state_set(rules::GameState* gs)
            { game_state_ = dynamic_cast<GameState*>(gs); }

    private:
        GameState* game_state_;
        rules::Player_sptr player_;
        rules::Actions actions_;

    public:

        ///
        // Retourne la nature du terrain désigné par ``pos``.
        //
        terrain info_terrain(position pos);
        ///
        // Retourne le joueur qui possède l'île à l'emplacement ``pos``.
        // Retourne -1 si la position indiquée n'est pas une île ou si l'île
        // est libre.
        //
        int info_ile_joueur(position pos);
        ///
        // Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne -1
        // si la case spécifiée n'est pas une île.
        //
        int info_ile_or(position pos);
        ///
        // Retourne le bateau ayant pour identifiant ``id``
        //
        bateau info_bateau(int id);
        ///
        // Retourne vrai si le bateau ayant pour identifiant ``id`` existe et est
        // encore à flots
        //
        bool bateau_existe(int id);
        ///
        // Retourne la liste de bateaux à la position ``pos``
        //
        std::vector<bateau> liste_bateaux_position(position pos);
        ///
        // Retourne la liste des ID des bateaux à la position ``pos``
        //
        std::vector<int> liste_id_bateaux_position(position pos);
        ///
        // Retourne la liste des positions des îles de la carte
        //
        std::vector<position> liste_iles();
        ///
        // Retourne la liste des positions des îles qui vous appartiennent
        //
        std::vector<position> mes_iles();
        ///
        // Retourne la distance entre deux positions
        //
        int distance(position depart, position arrivee);
        ///
        // Construire un bateau de type ``btype`` sur l'île à la position
        // ``pos``
        //
        erreur construire(bateau_type btype, position pos);
        ///
        // Déplace le bateau représenté par l'identifiant ``id`` jusqu'à la
        // position `pos`` (si elle est dans la portée du bateau)
        //
        erreur deplacer(int id, position pos);
        ///
        // Colonise l'île à la position ``pos``
        //
        erreur coloniser(position pos);
        ///
        // Charge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
        //
        erreur charger(int id, int nb_or);
        ///
        // Décharge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
        //
        erreur decharger(int id, int nb_or);
        ///
        // Retourne le numéro de votre joueur
        //
        int mon_joueur();
        ///
        // Retourne le numéro de votre adversaire
        //
        int adversaire();
        ///
        // Retourne les scores du joueur désigné par l'identifiant ``id``
        //
        int score(int id_joueur);
        ///
        // Retourne le numéro du tour actuel
        //
        int tour_actuel();
        ///
        // Return un dump JSON de l’état complet du jeu.
        //
        char* get_dump();
};


#endif /* !API_HH_ */
