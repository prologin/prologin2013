================================================
Finale Prologin 2013 − La conquête de l'archipel
================================================

------------
Introduction
------------

Le sujet de la finale de Prologin 2013 est un jeu de stratégie en tour par tour
où chaque joueur contrôle une flotte de bateaux qu'il doit utiliser pour
conquérir les îles de la carte.

Les joueurs vont pour cela coloniser des îles vierges et attaquer les îles des
autres joueurs, afin d'affirmer sa domination sur l'archipel et ainsi récolter
un maximum d'or et s'armer d'une gigantesque flotte.

------
Règles
------

Nombre de joueurs
=================

Une partie voit s'affronter 2 joueurs à la fois seulement.

Or
==

L'or est l'unité de base de monnaie du jeu. Vous pouvez gagner de l'or en
possédant des îles ou en attaquant des îles ennemies.
L'or est une ressource locale : il est contenu dans des îles et peut être
déplacé d'une île à une autre.


Bateaux
=======

Il est possible de construire des bateaux sur les îles colonisées.
Au départ, vous n'avez aucun bateau, vous devez vous charger vous même de la
construction.

Il existe deux types de bateaux, qui ont chacun des spécificités différentes.

La Caravelle
------------

La Caravelle peut coloniser des îles et déplacer de l'or. Elle doit être
utilisée sur chaque île vierge pour la coloniser.

* Coût : 3 or
* Déplacement : 4 cases

Le Galion
---------

Le Galion est un navire de combat. Il peut attaquer une flotte ou une île si
celle-ci se trouve sur la même case que lui.

* Coût : 1 or
* Déplacement : 6 cases

La carte
========

La carte est constituée de plusieurs éléments

* La mer : de grandes étendues d'eau sur lesquelles les vaisseaux peuvent se
  déplacer.
* Les îles : des morceaux de terre sur lesquels vous pouvez construire une
  colonie et construire des navires.
* Les volcans : des îles qui rapportent plus d'or que les îles classiques, mais
  ils sont trop dangereux pour y construire des bateaux.

Vous possédez au départ une île, votre point de départ. Vous pouvez coloniser
les autres îles de la carte.
Chaque île rapporte chaque jour un montant fixe d'or.
Deux îles peuvent être « collées » : elles sont toujours considérées comme deux
îles différentes, mais sans bras de mer les séparant.

Construction
============

Vous pouvez construire sur chaque île des bateaux si vous possédez assez d'or
sur l'île en question. Vous ne pouvez les déplacer qu'au tour suivant.

Déplacement
===========

Vous pouvez vous déplacer sur la mer et les îles. Un bateau ne peut
pas être déplacé plusieurs fois par tour, mais il peut être déplacé d'autant de
cases qu'il a dans sa portée.

Se déplacer en diagonale compte comme deux cases de déplacement.

Attaque
=======

Le vainqueur est celui qui a le plus de galions. En cas d'égalité, c'est
l'attaquant qui gagne si on est en mer, et le défenseur si on est sur une
île. Le vainqueur perd autant de galions que le perdant en avait, moins
un. Le perdant perd tout (galions, caravelles et contrôle de l'île).

Si le combat se déroule sur une île, l'île est attribuée au vainqueur et il
récupère l'or qui s'y trouvait. Si le combat se déroule en mer, l'or contenu
dans les caravelles du perdant est ajouté dans la caravelle du gagnant ayant
l'ID la plus petite. Si le gagnant n'a aucune caravelle, cet or est perdu.

-----
Score
-----

Le score de chaque joueur est déterminé à la fin de la partie comme étant la
totalité de l'or qu'il possède, plus le nombre de bateaux fois leurs coûts
respectifs, plus l'or que ses caravelles transportent.

-----------
Carte ASCII
-----------

La carte est représentée sous la forme d'un fichier texte utilisant la norme
suivante :

+-------+------------+
| ``~`` | Mer        |
+-------+------------+
| ``o`` | Île        |
+-------+------------+
| ``^`` | Volcan     |
+-------+------------+

Le fichier doit suivre le format suivant : ::

  depart_joueur1.x depart_joueur1.y
  depart_joueur2.x depart_joueur2.y
  ASCII map representation

Voici un exemple : ::

  1 1
  31 31
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~o~~~~~~~~~~~~~~o~~~~~~~~~~~~~~~
  ~~~~~~o~~~~o~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~o~~~o~~~~~~~~~~~~~~~~~~~~
  ~~o~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~
  ~~~~~~o~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~^~~~~~~~~~~~~~~~
  ~~~~~~~~~~~^~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~^~~~~~~~~^~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~^~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~^~~~~~~~~o~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~o~~~~~~~~~o~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~o~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~o~~~~o~~~~~~
  ~~~~~~~~~~~~~~~~o~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  ~~~~~~~~o~~~~o~~~~~~o~~~~~~~~~~~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~o~
  ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
