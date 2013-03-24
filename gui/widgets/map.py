# -*- coding: utf-8 -*-

from collections import defaultdict

import pygame

from api import *
import data
import game
import utils
from widgets.base import BaseWidget


class MapWidget(BaseWidget):

    def __init__(self, *args):
        super(MapWidget, self).__init__(*args)
        self.game_state = None
        self.position = None
        self.map_surface = None
        self.path_surface = None
        self.font = pygame.font.Font(data.get_font_path('font.ttf'), 12)
        self.cells_width = self.width / data.TILE_WIDTH
        self.cells_height = (self.height - data.TILE_OVERLAY) / (data.TILE_HEIGHT - data.TILE_OVERLAY)
        self.center = (self.cells_width / 2, self.cells_height / 2)

    def plug(self, widgets):
        self.details_widget = widgets['details']
        self.minimap = widgets['minimap']

    def handle_view_click(self, x, y, but1, but2, but3, absolute=False):
        x = x / data.TILE_WIDTH
        y = (y - data.TILE_OVERLAY) / (data.TILE_HEIGHT - data.TILE_OVERLAY)
        if not absolute:
            x += self.position[0]
            y += self.position[1]

        if but1:
            self.details_widget.update_position(x, y)
            self.update_subjective()
        elif but3:
            self.update_display((x - self.center[0], y - self.center[1]))

    def handle_click(self, x, y, but1, but2, but3):
        coords = self.is_click_inside(x, y)
        if not coords:
            return False
        if self.game_state is None:
            return True
        self.handle_view_click(coords[0], coords[1], but1, but2, but3)
        return True

    def update_game(self, game_state):
        self.game_state = game_state
        self.position_max = (
            utils.set_between(game_state.map_width - self.cells_width, lower=0),
            utils.set_between(game_state.map_width - self.cells_height, lower=0)
        )
        self.surface.fill(utils.BLACK)
        if self.game_state is None:
            return
        if not self.position:
            surf_size = (
                game_state.map_width * data.TILE_WIDTH,
                game_state.map_height * (data.TILE_HEIGHT - data.TILE_OVERLAY) + data.TILE_OVERLAY
            )
            self.map_surface = utils.make_surface(*surf_size)
            self.path_surface = utils.make_surface(*surf_size)
            self.position = (0, 0)

        water_pix = data.tiles['water']
        boat_pix = data.tiles['boat']
        cell_pix = {
            TERRAIN_ILE:    data.tiles['island'],
            TERRAIN_VOLCAN: data.tiles['volcano'],
        }

        # Collect the count of boats to display it on top of everything else.
        boat_counts = []
        players = self.game_state.players

        for y, row in enumerate(game_state.cells):
            for x, cell in enumerate(row):
                coords = (data.TILE_WIDTH * x, (data.TILE_HEIGHT - data.TILE_OVERLAY) * y)
                self.map_surface.blit(water_pix, coords)
                # Display the kind of the cell, if different than WATER.
                try:
                    self.map_surface.blit(cell_pix[cell.type], coords)
                except KeyError:
                    pass
                # Display settlement information, if any.
                if cell.player != game.NO_PLAYER:
                    self.map_surface.blit(
                        data.settlement[
                            game_state.player_id_to_order[cell.player]
                        ], coords
                    )
                # Display the number of boats per player.
                if cell.boats:
                    count_y = coords[1] + data.TILE_HEIGHT
                    count_per_player = defaultdict(lambda: 0)
                    for boat in cell.boats:
                        count_per_player[boat.player] += 1
                    ids = reversed(sorted(count_per_player))
                    self.map_surface.blit(boat_pix, coords)
                    for id, count in utils.iter_dict(count_per_player, ids):
                        boat_count = utils.make_bordered_text(
                            str(count), self.font,
                            fgcolor=data.get_player_color(game_state, id)
                        )
                        boat_count_w, boat_count_h = boat_count.get_size()
                        count_y -= boat_count_h
                        boat_counts.append((boat_count, (
                            coords[0] + data.TILE_WIDTH - boat_count_w,
                            count_y
                        )))
        for args in boat_counts:
            self.map_surface.blit(*args)

        self.update_display()

    def update_subjective(self):
        return self.update_display()

    def update_display(self, position=None):
        self.surface.fill(utils.BLACK)
        if position:
            x = utils.set_between(
                position[0],
                lower=0, upper=self.position_max[0]
            )
            y = utils.set_between(
                position[1],
                lower=0, upper=self.position_max[1]
            )
            self.position = (x, y)

        self.surface.blit(self.map_surface, (0, 0), (
                self.position[0] * data.TILE_WIDTH,
                self.position[1] * (data.TILE_HEIGHT - data.TILE_OVERLAY),
                self.width, self.height
        ))

        self.minimap.update_view()
