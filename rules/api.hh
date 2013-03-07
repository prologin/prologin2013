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
# define API_HH_

# include <vector>
# include <rules/game-state.hh>
# include <rules/player.hh>

# include "constant.hh"

/*!
** Method of this call are called by the candidat, throught 'interface.cc'
*/
class Api
{

public:
    Api(rules::GameState* game_state, rules::Player* player);
    virtual ~Api() { }

private:
    rules::GameState* game_state_;
    rules::Player* player_;

public:

///
// Retourne la taille de la carte.
//
   position taille_carte();
///
// Retourne la nature du terrain désigné par ``pos``.
//
   terrain info_terrain(position pos);
///
// Retourne l'équipe qui possède l'île à l'emplacement ``pos``. Retourne 0 si l'île est libre. Retourne -1 si la position indiquée n'est pas une île
//
   int info_ile_equipe(position pos);
///
// Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne 0 si la case spécifiée n'est pas une île.
//
   int info_ile_or(position pos);
///
// Retourne le bateau ayant pour identifiant ``id``
//
   bateau info_bateau(int id);
///
// Retourne la liste de bateaux à la position ``pos``
//
   std::vector<bateau> liste_bateaux_position(position pos);
///
// Retourne la liste des ID des bateaux à la position ``pos``
//
   std::vector<int> liste_id_bateaux_position(position pos);
///
// Retourne la liste des positions des îles
//
   std::vector<position> liste_iles();
///
// Retourne une liste de positions correspondant au plus court chemin d'un point à un autre en passant seulement par des cases accessibles
//
   std::vector<position> chemin();
///
// Construire un bateau de type ``btype`` sur l'île à la position ``pos``
//
   erreur construire(bateau_type btype, position pos);
///
// Déplace le bateau représenté par l'identifiant ``id`` suivant une liste ``liste_pos`` de positions
//
   erreur deplacer(int id, std::vector<position> liste_pos);
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
// Retourne les scores de chaque joueur
//
   std::vector<int> scores();
///
// Retourne le numéro du tour actuel
//
   int tour_actuel();
///
// Retourne le nombre total de tours de toute la partie
//
   int nombre_tours();
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

};


#endif /* !API_HH_ */