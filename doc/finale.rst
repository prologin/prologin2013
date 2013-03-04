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
un maximum d'or et de bâteaux.

------
Règles
------

Bateaux
=======

Il est possible de construire des bateaux sur les îles colonisées.
Chaque joueur peut construire un nombre limité (4) de bateaux par île et par
tour.

Il existe deux types de bateaux, qui ont chacun des spécificités différentes.

La Caravelle
------------

La Caravelle peut coloniser des îles et explorer des épaves. Elle doit être
utilisée sur chaque île vierge pour la coloniser.

* Coût : 25 or
* Déplacement : 4 cases

Le Galion
---------

Le Galion est un navire de combat. Il peut attaquer une flotte ou une île si
celle-ci se trouve sur la même case que lui.

* Coût : 10 or
* Déplacement : 6 cases

La carte
========

La carte est constituée de plusieurs éléments

* La mer : de grandes étendues d'eau sur lesquelles les vaisseaux peuvent se
  déplacer.
* Les îles : des morceaux de terre sur lesquels vous pouvez construire une
  colonie.
* Les récifs : des zones dangereuses où il est impossible de se déplacer.
* Les épaves : des navires coulés dont vous possédez l'emplacement, qui
  contiennent souvent de l'or.

Les îles
--------

Il existe trois sortes d'îles :

* Les îles classiques qui n'apportent aucun bonus ;
* Les îles-forêt qui réduisent le coût de construction des navires ;
* Les îles-volcan qui rapportent plus d'or à chaque tour.

Les épaves
----------

À chaque tour, si vous avez des caravelles présentes sur une épave, vous pouvez
les explorer et ainsi rapporter (le nombre de caravelles) × REVENU_EPAVE en or.

Chaque épave a un nombre d'or limité EPAVE_OR_MAX.

Construction
============

Vous pouvez construire sur chaque île des bateaux (un nombre limité de 4 par
île et par tour) si vous possédez assez d'or. Vous ne pouvez les déplacer
qu'au tour suivant.

Déplacement
===========

Vous pouvez vous déplacer sur la mer, les îles et les épaves. Un bateau ne peut
pas être déplacé plusieurs fois par tour, mais il peut être déplacé d'autant de
cases qu'il a dans sa portée.

Attaque
=======

Si deux ou plusieurs flottes sont sur la même case, à la fin de chaque tour,
une attaque se déclenche automatiquement. Chaque flotte perd autant de gallions
que possède la plus grande flotte ennemie, si bien qu'il ne reste plus qu'une
flotte présente.

À la fin du combat :

* Les perdants perdent également toutes leurs caravelles.
* Si le combat se déroulait sur une île appartenant à un des perdants, il la
  perd et elle devient vierge.
* Le gagnant reçoit la moitié de chacun du prix des vaisseaux vaincus.

-----
Score
-----

Le score de chaque joueur est déterminé à la fin de la partie comme étant la
totalité de l'or qu'il possède, plus le nombre de bateaux qu'il possède fois la
moitié de leur coûts respectifs.

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
| ``^`` | Île-volcan |
+-------+------------+
| ``*`` | Île-forêt  |
+-------+------------+
| ``x`` | Épave      |
+-------+------------+


Le fichier doit suivre le format suivant : ::

  height width
  maximum number of turns
  ASCII map representation
  depart_joueur1.x depart_joueur1.y
  depart_joueur2.x depart_joueur2.y
  ...
  depart_joueurN.x depart_joueurN.y

Voici un exemple : ::

  8 10
  200
  ~~o~~~~~~~~~~~~~~~~~
  ~~~~~~~~~~~~*~~~~~x~
  ~~o~~~~o~~~~~~~~~~~~
  ~~~~~~~~~~~~o~~~~~~~
  ~~~^~~x~~x~~x~~^~~~~
  ~~~~~~~~~~o~~~~~~~~~
  ~~~*~~~~~~~~~o~~~~~~
  ~x~~~~~~~~o~~~~~~~~~
  ~~~~~~~~~~~~~~~~~o~~
  2 0
  7 7
