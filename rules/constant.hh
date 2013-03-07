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

# define                           

# define                           

# define                           

# define                           

# define                           

# define                           

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
  TERRAIN_RECIF, /* <- Récif */
  TERRAIN_ERREUR, /* <- Erreur, case impossible */
} terrain;


///
// Erreurs possibles
//
typedef enum erreur {
  OR_INSUFFISANT, /* <- Vous ne possédez pas assez d'or pour cette action. */
  ILE_INVALIDE, /* <- La position spécifiée n'est pas une île. */
  CHEMIN_INVALIDE, /* <- Le chemin spécifié n'est pas valide. */
  BATEAUX_INSUFFISANTS, /* <- Il n'y a pas assez de bateaux de ce type sur la case spécifiée */
  ILE_COLONISEE, /* <- L'île est déjà colonisée */
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
  int equipe;  /* <- Équipe */
  bateau_type type;  /* <- Type */
  int or;  /* <- Or contenu dans le bateau */
  bool deplacable;  /* <- Le bateau n'a pas encore été déplacé ce tour-ci */
  bool vivant;  /* <- Le bateau est encore à flots */
} bateau;



#endif // !CONSTANT_HH_
