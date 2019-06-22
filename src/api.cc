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

#include <algorithm>
#include <stdlib.h>

#include "api.hh"
#include "dumper.hh"
#include "tools.hh"

#include "action-charge.hh"
#include "action-colonize.hh"
#include "action-construct.hh"
#include "action-discharge.hh"
#include "action-move.hh"
#include "action-transfer.hh"

// global used in interface.cc
Api* api;

Api::Api(std::unique_ptr<GameState> game_state, rules::Player_sptr player)
    : rules::Api<GameState, erreur>(std::move(game_state), player)
{
    api = this;
}

///
// Retourne la nature du terrain désigné par ``pos``.
//
terrain Api::info_terrain(position pos)
{
    Cell* c = game_state_->get_map()->get_cell(pos);
    if (c)
        return c->get_type();
    return TERRAIN_ERREUR;
}

///
// Retourne le joueur qui possède l'île à l'emplacement ``pos``.
// Retourne -1 si la position indiquée n'est pas une île ou si l'île est libre
//
int Api::info_ile_joueur(position pos)
{
    Cell* c = game_state_->get_map()->get_cell(pos);
    if (c)
        return c->get_player();
    return -1;
}

///
// Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne -1 si la
// case spécifiée n'est pas une île.
//
int Api::info_ile_or(position pos)
{
    Cell* c = game_state_->get_map()->get_cell(pos);
    if (c)
        return c->get_gold();
    return -1;
}

///
// Retourne le bateau ayant pour identifiant ``id``
//
bateau Api::info_bateau(int id)
{
    bateau* boat = game_state_->get_boat(id);
    if (boat != NULL)
        return *boat;
    return {-1, {-1, -1}, -1, BATEAU_ERREUR, -1, false};
}

///
// Retourne vrai si le bateau ayant pour identifiant ``id`` existe et est
// encore à flots
//
bool Api::bateau_existe(int id)
{
    return (game_state_->get_boat(id) != NULL);
}

///
// Retourne la liste de bateaux à la position ``pos``
//
std::vector<bateau> Api::liste_bateaux_position(position pos)
{
    Cell* c = game_state_->get_map()->get_cell(pos);
    std::vector<bateau> r;
    if (!c)
        return r;
    for (auto& i : c->get_id_boats())
        r.push_back(*game_state_->get_boat(i));
    return r;
}

///
// Retourne la liste des ID des bateaux à la position ``pos``
//
std::vector<int> Api::liste_id_bateaux_position(position pos)
{
    Cell* c = game_state_->get_map()->get_cell(pos);
    if (!c)
        return std::vector<int>();
    std::set<int> i = c->get_id_boats();
    std::vector<int> r(i.size());
    std::copy(i.begin(), i.end(), r.begin());
    return r;
}

///
// Retourne la liste des positions des îles de la carte
//
std::vector<position> Api::liste_iles()
{
    return game_state_->get_map()->get_islands();
}

///
// Retourne la liste des positions des îles qui vous appartiennent
//
std::vector<position> Api::mes_iles()
{
    std::vector<position> r;
    std::vector<position> i = game_state_->get_map()->get_islands();
    for (auto c : i)
        if (game_state_->get_map()->get_cell(c)->get_player() ==
            (int)player_->id)
            r.push_back(c);
    return r;
}

///
// Retourne l'ID du dernier bateau construit. Son comportement n'est pas défini
// si vous n'avez pas encore créé de bateau à ce tour-ci.
//
int Api::id_dernier_bateau_construit()
{
    return game_state_->get_last_id();
}

///
// Retourne la distance entre deux positions
//
int Api::distance(position depart, position arrivee)
{
    return ::distance(depart, arrivee);
}

///
// Retourne le numéro de votre joueur
//
int Api::mon_joueur()
{
    return player_->id;
}

///
// Retourne le numéro de votre adversaire
//
int Api::adversaire()
{
    return game_state_->get_opponent(player_->id);
}

///
// Retourne les scores du joueur spécifié
//
int Api::score(int id_joueur)
{
    if (!game_state_->player_exists(id_joueur))
        return 0;
    return game_state_->get_score(id_joueur);
}

///
// Retourne le numéro du tour actuel
//
int Api::tour_actuel()
{
    return game_state_->get_current_round();
}

///
// Retourne le nombre de bateaux que possède le joueur désigné par
// l'identifiant ``id``
//
int Api::nombre_bateaux(int id_joueur)
{
    if (!game_state_->player_exists(id_joueur))
        return 0;
    return game_state_->get_nb_boats(id_joueur);
}

///
// Return un dump JSON de l’état complet du jeu.
//
char* Api::get_dump()
{
    return dump_game_state(*game_state_, actions_);
}
