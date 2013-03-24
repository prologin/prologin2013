///
// This file has been generated, if you wish to
// modify it in a permanent way, please refer
// to the script file : gen/generator_python.rb
//

#ifndef INTERFACE_HH_
# define INTERFACE_HH_

# include <Python.h>
# include <vector>
# include <string>

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



extern "C" {

///
// Retourne la nature du terrain désigné par ``pos``.
//
terrain api_info_terrain(position pos);

///
// Retourne le joueur qui possède l'île à l'emplacement ``pos``. Retourne -1 si l'île est libre ou si la position indiquée n'est pas une île
//
int api_info_ile_joueur(position pos);

///
// Retourne l'or contenu sur l'île à l'emplacement ``pos``. Retourne -1 si la case spécifiée n'est pas une île.
//
int api_info_ile_or(position pos);

///
// Retourne le bateau ayant pour identifiant ``id``
//
bateau api_info_bateau(int id);

///
// Retourne vrai si le bateau ayant pour identifiant ``id`` existe et est encore à flots
//
bool api_bateau_existe(int id);

///
// Retourne la liste de bateaux à la position ``pos``
//
std::vector<bateau> api_liste_bateaux_position(position pos);

///
// Retourne la liste des ID des bateaux à la position ``pos``
//
std::vector<int> api_liste_id_bateaux_position(position pos);

///
// Retourne la liste des positions des îles de la carte
//
std::vector<position> api_liste_iles();

///
// Retourne la liste des positions des îles qui vous appartiennent
//
std::vector<position> api_mes_iles();

///
// Retourne la distance entre deux positions
//
int api_distance(position depart, position arrivee);

///
// Construire un bateau de type ``btype`` sur l'île à la position ``pos``
//
erreur api_construire(bateau_type btype, position pos);

///
// Déplace le bateau représenté par l'identifiant ``id`` jusqu'à la position `pos`` (si elle est dans la portée du bateau)
//
erreur api_deplacer(int id, position pos);

///
// Colonise l'île à la position ``pos``
//
erreur api_coloniser(position pos);

///
// Charge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
erreur api_charger(int id, int nb_or);

///
// Décharge la caravelle identifiée par ``id`` de ``nb_or`` d'or.
//
erreur api_decharger(int id, int nb_or);

///
// Retourne le numéro de votre joueur
//
int api_mon_joueur();

///
// Retourne le numéro de votre adversaire
//
int api_adversaire();

///
// Retourne les scores du joueur désigné par l'identifiant ``id``
//
int api_score(int id_joueur);

///
// Retourne le numéro du tour actuel
//
int api_tour_actuel();

///
// Affiche le contenu d'une valeur de type bateau_type
//
void api_afficher_bateau_type(bateau_type v);

///
// Affiche le contenu d'une valeur de type terrain
//
void api_afficher_terrain(terrain v);

///
// Affiche le contenu d'une valeur de type erreur
//
void api_afficher_erreur(erreur v);

///
// Affiche le contenu d'une valeur de type position
//
void api_afficher_position(position v);

///
// Affiche le contenu d'une valeur de type bateau
//
void api_afficher_bateau(bateau v);

char* api_get_dump();

}

#endif // !INTERFACE_HH_
