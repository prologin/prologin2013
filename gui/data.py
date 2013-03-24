# -*- coding: utf-8 -*-

import os.path
import re

import pygame

import api
import utils


paths = {
    "prefix": "/home/pmderodat/usr",
    "exec_prefix": "${prefix}",
    "sysconfdir": "${prefix}/etc",
    "imagesdir": "${prefix}/share/stechec/prolo2011/graphics",
    "cfgdir": "${prefix}/etc/stechec",
    "bindir": "${exec_prefix}/bin",
}

def expand_path(p):
    return re.sub(r'\$\{(.*?)\}', lambda m: expand_path(paths[m.group(1)]), p)

# paths = {name: expand_path(path) for name, path in paths.iteritems()}

# images_dir = paths['imagesdir']

images_dir = os.path.join(os.path.dirname(os.path.abspath(__file__)), 'data')

def get_image_path(name):
    return os.path.join(images_dir, name)

def get_font_path(name):
    return os.path.join(images_dir, name)


PLAYER_COLORS = [
    (0, 128, 192),
    (192, 0, 0),
    (0, 192, 0),
]

def get_player_color(game_state, player_id):
    player_order = game_state.player_id_to_order[player_id]
    return PLAYER_COLORS[player_order]

def get_colored_images(pattern):
    return [
        utils.set_color(pattern, color)
        for color in PLAYER_COLORS
    ]

TILE_WIDTH = 50
TILE_HEIGHT = 50
TILE_OVERLAY = 0

ICON_WIDTH = 32
ICON_HEIGHT = 32

tiles = None
icons = None
settlement = None

def get_images(names):
    result = {}
    for name in names:
        path = get_image_path('{}.png'.format(name))
        img = pygame.image.load(path).convert_alpha()
        result[name] = img
    return result

def load_images():
    global tiles, icons, settlement, icon_shadow
    tiles = get_images('water island volcano settlement boat'.split())
    icons = get_images('caravelle galion gold'.split())
    settlement = get_colored_images(tiles['settlement'])

    icon_shadow = utils.make_shadow(ICON_WIDTH, ICON_HEIGHT)
