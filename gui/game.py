# -*- coding: utf-8 -*-

from collections import namedtuple
import struct

from api import *

Cell = namedtuple('Cell', 'type gold player boats')
Boat = namedtuple('Boat', 'type gold player')
Player = namedtuple('Player', 'name score')

BOAT_TYPES = {
    BATEAU_CARAVELLE:   u'caravelle',
    BATEAU_GALION:      u'galion',
}

CELL_TYPES = {
    TERRAIN_ILE:        u'île',
    TERRAIN_VOLCAN:     u'volcan',
    TERRAIN_MER:        u'mer',
}

# TODO
action_id = struct.Struct('i')
action_ack = struct.Struct('i') # Player_id
action_attack = struct.Struct(
    'iiii'  # perso_info(equipe, classe, vie, direction)
    'i'     # attaque_type
    'ii'    # position
)
action_move_first = struct.Struct(
    'iiii'  # perso_info(equipe, classe, vie, direction)
    'i'     # path_length
)
action_move_path_element = struct.Struct('ii') # position
action_move_last = struct.Struct(
    'ii'    # direction and player
)

NO_PLAYER = -1
DEFAULT_CELL = Cell(TERRAIN_MER, 0, -1, [])

def cell_from_json(cell):
    return Cell(cell['type'], cell['gold'], cell['player'], [])

def boat_from_json(boat):
    return Boat(boat['type'], boat['gold'], boat['player'])

class GameState(object):

    def __init__(self, json):
        self.turn = json['turn'][0]
        self.turn_max = json['turn'][1]

        map = json['map']
        self.map_width = map['width']
        self.map_height = map['height']
        self.cells = [
            [
                DEFAULT_CELL._replace(boats=[])
                for _ in range(map['width'])
            ]
            for _ in range(map['height'])
        ]

        for cell in map['cells']:
            x, y = cell['x'], cell['y']
            self.cells[y][x] = cell_from_json(cell)
        for boat in map['boats']:
            x, y = boat['x'], boat['y']
            self.cells[y][x].boats.append(boat_from_json(boat))

        self.players = {}
        self.player_id_to_order = {}
        ordered_players_ids = []
        for id, player in json['players'].items():
            id = int(id)
            self.players[id] = Player(
                player['name'] or (u'Équipe %d' % id),
                player['score']
            )
            self.player_id_to_order[id] = len(ordered_players_ids)
            ordered_players_ids.append(id)
        self.ordered_players_ids = sorted(ordered_players_ids)
