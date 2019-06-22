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

#include <rules/actions.hh>
#include <rules/api.hh>
#include <rules/game-state.hh>
#include <rules/player.hh>

#include "action-charge.hh"
#include "action-colonize.hh"
#include "action-construct.hh"
#include "action-discharge.hh"
#include "action-move.hh"
#include "action-transfer.hh"
#include "constant.hh"
#include "game.hh"

/*!
 ** Method of this call are called by the candidat, throught 'interface.cc'
 */
class Api final : public ::rules::Api<GameState, erreur>
{
public:
    Api(std::unique_ptr<GameState> game_state, rules::Player_sptr player);

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
    // Retourne l'ID du dernier bateau construit. Son comportement n'est pas
    // défini si vous n'avez pas encore créé de bateau à ce tour-ci.
    //
    int id_dernier_bateau_construit();
    ///
    // Retourne la distance entre deux positions
    //
    int distance(position depart, position arrivee);
    ///
    // Construire un bateau de type ``btype`` sur l'île à la position
    // ``pos``
    //
    ApiActionFunc<ActionConstruct> construire{this};
    ///
    // Déplace le bateau représenté par l'identifiant ``id`` jusqu'à la
    // position `pos`` (si elle est dans la portée du bateau)
    //
    ApiActionFunc<ActionMove> deplacer{this};
    ///
    // Colonise l'île à la position ``pos``
    //
    ApiActionFunc<ActionColonize> coloniser{this};
    ///
    // Charge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
    //
    ApiActionFunc<ActionCharge> charger{this};
    ///
    // Décharge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
    //
    ApiActionFunc<ActionDischarge> decharger{this};
    ///
    // Transfère ``montant`` or de la caravelle ``id_source`` à la
    // caravelle ``id_dest``
    //
    ApiActionFunc<ActionTransfer> transferer{this};
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
    // Retourne le nombre de bateaux que possède le joueur désigné par
    // l'identifiant ``id``
    //
    int nombre_bateaux(int id_joueur);
    ///
    // Return un dump JSON de l’état complet du jeu.
    //
    char* get_dump();
};

#endif /* !API_HH_ */
