# -*- coding: iso-8859-1 -*-

try:
    from _api import *
except ImportError:
    pass

# Taille du terrain (longueur et largeur)
TAILLE_TERRAIN = 32

# Nombre de tours par partie
FIN_PARTIE = 200

# Nombre maximum de joueurs dans la partie
MAX_JOUEURS = 2

# Revenu en or par île à chaque tour
REVENU_ILE = 10

# Revenu en or par volcan à chaque tour
REVENU_VOLCAN = 20

# Coût de construction d'une Caravelle
CARAVELLE_COUT = 3

# Coût de construction d'un Galion
GALION_COUT = 1

# Déplacement de la Caravelle
CARAVELLE_DEPLACEMENT = 4

# Déplacement du Galion
GALION_DEPLACEMENT = 6

# Type de bateau
(
    BATEAU_CARAVELLE, # <- Caravelle
    BATEAU_GALION, # <- Galion
    BATEAU_ERREUR, # <- Bateau inexistant
) = range(3)


# Type de terrain
(
    TERRAIN_ILE, # <- Île
    TERRAIN_VOLCAN, # <- Volcan
    TERRAIN_MER, # <- Mer
    TERRAIN_ERREUR, # <- Erreur, case impossible
) = range(4)


# Erreurs possibles
(
    OK, # <- L'action a été exécutée avec succès
    OR_INSUFFISANT, # <- Vous ne possédez pas assez d'or pour cette action
    ILE_INVALIDE, # <- La position spécifiée n'est pas une île
    POSITION_INVALIDE, # <- La position spécifiée n'est pas valide
    TROP_LOIN, # <- La destination est trop éloignée
    ILE_COLONISEE, # <- L'île est déjà colonisée
    ILE_ENNEMIE, # <- L'île ne vous appartient pas
    BATEAU_ENNEMI, # <- L'île ne vous appartient pas
    ID_INVALIDE, # <- L'ID spécifiée n'est pas valide
    NON_DEPLACABLE, # <- Le bateau n'est pas déplaçable
    AUCUNE_CARAVELLE, # <- Il n'y a aucune caravelle susceptible de coloniser l'île
) = range(11)


from collections import namedtuple

# Représente la position sur la carte
position = namedtuple("position",
    'x ' # <- Coordonnée en X
    'y ' # <- Coordonnée en Y
)


# Représente un bateau
bateau = namedtuple("bateau",
    'id ' # <- Identifiant unique du bateau
    'pos ' # <- Position
    'joueur ' # <- Joueur
    'btype ' # <- Type
    'nb_or ' # <- Or contenu dans le bateau
    'deplacable ' # <- Le bateau n'a pas encore été déplacé ce tour-ci
)


