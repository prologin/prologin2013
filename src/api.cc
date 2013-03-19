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

#include "action-charge.hh"
#include "action-construct.hh"
#include "action-colonize.hh"
#include "action-discharge.hh"
#include "action-move.hh"

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
    std::map<int, bateau> boats = game_state_->get_boats();
    if (boats.find(id) != boats.end())
        return boats[id];
    return {-1, {-1, -1}, -1, BATEAU_ERREUR, -1, false};
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
    for(auto& i: c->get_id_boats())
        r.push_back(game_state_->get_boats()[i]);
    return r;
}

///
// Retourne la liste des ID des bateaux à la position ``pos``
//
std::vector<int> Api::liste_id_bateaux_position(position pos)
{
    Cell* c = game_state_->get_map()->get_cell(pos);
    std::vector<int> r;
    if (!c)
        return r;
    std::set<int> i = c->get_id_boats();
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
            (int) player_->id; });
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
    rules::IAction_sptr action(new ActionConstruct(btype, pos, player_->id));
    
    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    return OK;
}

///
// Déplace le bateau représenté par l'identifiant ``id`` jusqu'à la position `pos`` (si elle est dans la portée du bateau)
//
erreur Api::deplacer(int id, position pos)
{
    rules::IAction_sptr action(new ActionMove(id, pos, player_->id));
    
    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    return OK;
}

///
// Colonise l'île à la position ``pos``
//
erreur Api::coloniser(position pos)
{
    rules::IAction_sptr action(new ActionColonize(pos, player_->id));
    
    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    return OK;
}

///
// Charge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
erreur Api::charger(int id, int nb_or)
{
    rules::IAction_sptr action(new ActionCharge(id, nb_or, player_->id));
    
    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    return OK;
}

///
// Décharge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
erreur Api::decharger(int id, int nb_or)
{
    rules::IAction_sptr action(new ActionDischarge(id, nb_or, player_->id));
    
    erreur err;
    if ((err = static_cast<erreur>(action->check(game_state_))) != OK)
        return err;

    actions_.add(action);
    return OK;
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
