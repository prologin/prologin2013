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

#include "api.hh"

// global used in interface.cc
Api* api;

Api::Api(rules::GameState* game_state, rules::Player* player)
    : game_state_(game_state),
      player_(player)
{
  api = this;
}

///
// Retourne la taille de la carte.
//
position Api::taille_carte()
{
  // TODO
  abort();
}

///
// Retourne la nature du terrain désigné par ``pos``.
//
terrain Api::info_terrain(position pos)
{
  // TODO
  abort();
}

///
// Retourne l'équipe qui possède l'île à l'emplacement ``pos``. Retourne 0 si l'île est libre. Retourne -1 si la position indiquée n'est pas une île
//
int Api::info_ile_equipe(position pos)
{
  // TODO
  abort();
}

///
// Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne -1 si la case spécifiée n'est pas une île.
//
int Api::info_ile_or(position pos)
{
  // TODO
  abort();
}

///
// Retourne le bateau ayant pour identifiant ``id``
//
bateau Api::info_bateau(int id)
{
  // TODO
  abort();
}

///
// Retourne la liste de bateaux à la position ``pos``
//
std::vector<bateau> Api::liste_bateaux_position(position pos)
{
  // TODO
  abort();
}

///
// Retourne la liste des ID des bateaux à la position ``pos``
//
std::vector<int> Api::liste_id_bateaux_position(position pos)
{
  // TODO
  abort();
}

///
// Retourne la liste des positions des îles de la carte
//
std::vector<position> Api::liste_iles()
{
  // TODO
  abort();
}

///
// Retourne la liste des positions des îles qui vous appartiennent
//
std::vector<position> Api::mes_iles()
{
  // TODO
  abort();
}

///
// Retourne une liste de positions correspondant au plus court chemin d'un point à un autre en passant seulement par des cases accessibles
//
std::vector<position> Api::chemin()
{
  // TODO
  abort();
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
// Déplace le bateau représenté par l'identifiant ``id`` suivant une liste ``liste_pos`` de positions
//
erreur Api::deplacer(int id, std::vector<position> liste_pos)
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
  // TODO
  abort();
}

///
// Retourne les scores de chaque joueur
//
std::vector<int> Api::scores()
{
  // TODO
  abort();
}

///
// Retourne le numéro du tour actuel
//
int Api::tour_actuel()
{
  // TODO
  abort();
}

///
// Retourne le nombre total de tours de toute la partie
//
int Api::nombre_tours()
{
  // TODO
  abort();
}

///
// Affiche le contenu d'une valeur de type bateau_type
//

///
// Affiche le contenu d'une valeur de type terrain
//

///
// Affiche le contenu d'une valeur de type erreur
//

///
// Affiche le contenu d'une valeur de type position
//

///
// Affiche le contenu d'une valeur de type bateau
//

