=========
Stechec 2
=========

Site
====

Champion
--------

Pour lancer un match il va vous falloir plusieurs composants. Le premier
est un champion. Pour envoyer vos champion sur le site, vous pouvez aller sur
la rubrique appropriée.

Attention, il ne faut pas envoyé le fichier .so mais une archive du dossier
dans lequel vous travaillez. Pour ce faire il vous suffit de taper la
commande suivante :

    make tar

Cela va créer un fichier `champion.tgz`. C'est ce fichier que vous devez
donner au site sur la page « Envoyer un champion ».

Match
-----

Une fois cette étape réalisée, vous aller pouvoir lancer un match. Sur la
dédiée, vous aller devoir préciser les identifiants de deux champions que vous
souhaitez utiliser. Vous pouvez les trouver dans « Mes champions » pour les
votre, et dans « Tous les champions » si vous souhaitez vous mesurer au
candidat d'un autre champion.

Cartes
------

Vous pouvez aussi uploader des cartes si jamais vous ne souhaitez pas utiliser
les cartes déjà présentes.

Fichier de configuration
========================

Pour lancer un match avec Stechec 2 vous avez besoin d'un fichier de
configuration pour indiquer au serveur et aux clients quelles règles choisir.
De plus le serveur a besoin de savoir combien de clients vont se connecter
avant de lancer le match, et le client a besoin de savoir où est vore champion
pour le charger.

Prenons un exemple (qui devrait par ailleurs satisfaire vos besoin) ::

  rules: libprologin2013.so
  map: ./simple.map
  verbose: 3
  clients:
    - ./champion.so
    - ./champion.so
  names:
    - Blue ones
    - Red ones

Vous devez lancer depuis votre ligne de commande stechec2 : ::

    prompt$ stechec2-run.py config.yml

En cas de bug dans stechec2, n'hésitez pas à les rapporter.

Gui
---

Pour utiliser la GUI pendant que vous lancez votre match, rajoutez à la fin de
votre config.yml::

  spectators:
   - /usr/local/gui/gui.so

Si vous préférez regarder le replay à la fin::

  spectators:
   - /usr/local/gui/dumper.so

Ceci crée un fichier dump.json dans le répertoire courant, que vous pouvez
visualiser avec::

  /usr/local/gui/replay.py dump.json

Vous pouvez évidemment utiliser les deux en même temps::

  spectators:
   - /usr/local/gui/gui.so
   - /usr/local/gui/dumper.so
