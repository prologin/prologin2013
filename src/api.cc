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

#include <stdlib.h>
#include <algorithm>

#include "api.hh"
#include "tools.hh"

// global used in interface.cc
Api* api;

Api::Api(GameState* game_state, rules::Player_sptr player)
    : game_state_(game_state),
      player_(player)
{
  api = this;
}

///
// Retourne la nature du terrain désigné par ``pos``.
//
terrain Api::info_terrain(position pos)
{
    return game_state_->get_map()->get_cell(pos)->get_type();
}

///
// Retourne le joueur qui possède l'île à l'emplacement ``pos``. Retourne 0 si l'île est libre. Retourne -1 si la position indiquée n'est pas une île
//
int Api::info_ile_joueur(position pos)
{
    return game_state_->get_map()->get_cell(pos)->get_player();
}

///
// Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne -1 si la case spécifiée n'est pas une île.
//
int Api::info_ile_or(position pos)
{
    return game_state_->get_map()->get_cell(pos)->get_gold();
}

///
// Retourne le bateau ayant pour identifiant ``id``
//
bateau Api::info_bateau(int id)
{
    return game_state_->get_boats()[id];
}

///
// Retourne la liste de bateaux à la position ``pos``
//
std::vector<bateau> Api::liste_bateaux_position(position pos)
{
    std::vector<bateau> r;
    for(auto& i: game_state_->get_map()->get_cell(pos)->get_id_boats())
        r.push_back(game_state_->get_boats()[i]);
    return r;
}

///
// Retourne la liste des ID des bateaux à la position ``pos``
//
std::vector<int> Api::liste_id_bateaux_position(position pos)
{
    std::vector<int> r;
    std::set<int> i = game_state_->get_map()->get_cell(pos)->get_id_boats();
    std::copy(i.begin(), i.end(), std::back_inserter(r));
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
    std::copy_if(i.begin(), i.end(), r.begin(), [this](position p) {
            return game_state_->get_map()->get_cell(p)->get_player() ==
            player_->id; });
    return r;
}

///
// Retourne la distance entre deux positions
//
int Api::distance(position depart, position arrivee)
{
    return distance(depart, arrivee);
}

///
// Construire un bateau de type ``btype`` sur l'île à la position ``pos``
//
erreur Api::construire(bateau_type btype, position pos)
{
  // TODO
  abort();
}

///
// Déplace le bateau représenté par l'identifiant ``id`` jusqu'à la position `pos`` (si elle est dans la portée du bateau)
//
erreur Api::deplacer(int id, position pos)
{
  // TODO
  abort();
}

///
// Colonise l'île à la position ``pos``
//
erreur Api::coloniser(position pos)
{
  // TODO
  abort();
}

///
// Charge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
erreur Api::charger(int id, int nb_or)
{
  // TODO
  abort();
}

///
// Décharge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
erreur Api::decharger(int id, int nb_or)
{
  // TODO
  abort();
}

///
// Retourne le numéro de votre joueur
//
int Api::mon_joueur()
{
    return player_->id;
}

///
// Retourne les scores de chaque joueur
//
std::vector<int> Api::scores()
{
    return game_state_->get_scores();
}

///
// Retourne le numéro du tour actuel
//
int Api::tour_actuel()
{
    return game_state_->get_current_turn();
}
