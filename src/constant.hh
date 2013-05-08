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

#ifndef CONSTANT_HH_
# define CONSTANT_HH_

///
// Taille du terrain (longueur et largeur)
//
# define TAILLE_TERRAIN            32

///
// Nombre de tours par partie
//
# define FIN_PARTIE                200

///
// Nombre maximum de joueurs dans la partie
//
# define MAX_JOUEURS               2

///
// Revenu en or par île à chaque tour
//
# define REVENU_ILE                5

///
// Revenu en or par volcan à chaque tour
//
# define REVENU_VOLCAN             10

///
// Or initialement possédé par chaque joueur
//
# define OR_INITIAL                20

///
// Coût de construction d'une Caravelle
//
# define CARAVELLE_COUT            15

///
// Coût de construction d'un Galion
//
# define GALION_COUT               4

///
// Déplacement de la Caravelle
//
# define CARAVELLE_DEPLACEMENT     4

///
// Déplacement du Galion
//
# define GALION_DEPLACEMENT        6

///
// Type de bateau
//
typedef enum bateau_type {
  BATEAU_CARAVELLE, /* <- Caravelle */
  BATEAU_GALION, /* <- Galion */
  BATEAU_ERREUR, /* <- Bateau inexistant */
} bateau_type;


///
// Type de terrain
//
typedef enum terrain {
  TERRAIN_ILE, /* <- Île */
  TERRAIN_VOLCAN, /* <- Volcan */
  TERRAIN_MER, /* <- Mer */
  TERRAIN_ERREUR, /* <- Erreur, case impossible */
} terrain;


///
// Erreurs possibles
//
typedef enum erreur {
  OK, /* <- L'action a été exécutée avec succès */
  OR_INSUFFISANT, /* <- Vous ne possédez pas assez d'or pour cette action */
  ILE_INVALIDE, /* <- La position spécifiée n'est pas une île */
  POSITION_INVALIDE, /* <- La position spécifiée n'est pas valide */
  TROP_LOIN, /* <- La destination est trop éloignée */
  ILE_COLONISEE, /* <- L'île est déjà colonisée */
  ILE_ENNEMIE, /* <- L'île ne vous appartient pas */
  BATEAU_ENNEMI, /* <- L'île ne vous appartient pas */
  ID_INVALIDE, /* <- L'ID spécifiée n'est pas valide */
  NON_DEPLACABLE, /* <- Le bateau n'est pas déplaçable */
  AUCUNE_CARAVELLE, /* <- Il n'y a aucune caravelle susceptible de coloniser l'île */
} erreur;


///
// Représente la position sur la carte
//
typedef struct position {
  int x;  /* <- Coordonnée en X */
  int y;  /* <- Coordonnée en Y */
} position;


///
// Représente un bateau
//
typedef struct bateau {
  int id;  /* <- Identifiant unique du bateau */
  position pos;  /* <- Position */
  int joueur;  /* <- Joueur */
  bateau_type btype;  /* <- Type */
  int nb_or;  /* <- Or contenu dans le bateau */
  bool deplacable;  /* <- Le bateau n'a pas encore été déplacé ce tour-ci */
} bateau;



#endif // !CONSTANT_HH_
