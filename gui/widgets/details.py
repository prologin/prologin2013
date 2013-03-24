# -*- coding: utf-8 -*-

from collections import namedtuple

import pygame

from api import *

import data
import game
import utils
from widgets.scrolled import ScrolledWidget

class StatsAggregate:

    ICONS = 'gold galion caravelle'.split()

    def __init__(self):
        self.gold = 0
        self.galion_count = 0
        self.caravelle_count = 0

    def update(self, boat):
        self.gold += boat.gold
        if boat.type == BATEAU_GALION:
            self.galion_count += 1
        elif boat.type == BATEAU_CARAVELLE:
            self.caravelle_count += 1

    def __iter__(self):
        return iter(zip(
            self.ICONS, (self.gold, self.galion_count, self.caravelle_count)
        ))

class DetailsWidget(ScrolledWidget):
    PADDING = 8
    LINE_HEIGHT = 55

    def __init__(self, *args):
        self.font = pygame.font.Font(data.get_font_path('font.ttf'), 12)
        super(DetailsWidget, self).__init__(*args)
        self.line_shadow = utils.make_shadow(self.width, self.LINE_HEIGHT)
        self.game_state = None
        self.position = None
        self.selection = None

    def plug(self, widgets):
        self.map_widget = widgets['map']

    def update_game(self, game):
        self.game_state = game
        if self.position:
            self.update_position(*self.position)

    def update_position(self, x, y, selection=None):
        if not self.game_state:
            return
        if (
            x < 0 or self.game_state.map_width <= x or
            y < 0 or self.game_state.map_height <= y
        ):
            self.list_surface = None
            self.scroll(0)
            return

        self.position = (x, y)
        self.selection = selection
        cell = self.game_state.cells[y][x]
        self.set_list_length(1 + len(self.game_state.players))
        self._display_cell(x, y, cell)

        stats_per_player = {
            id: StatsAggregate()
            for id in self.game_state.players
        }
        for boat in cell.boats:
            stats_per_player[boat.player].update(boat)

        for i, (id, stats) in enumerate(utils.iter_dict(
            stats_per_player,
            self.game_state.ordered_players_ids
        ), 1):
            entry_y = self.PADDING + i * self.LINE_HEIGHT
            self._display_player_stats(id, stats, entry_y)

        self.scroll(0)

    def _display_cell(self, x, y, cell):
        # Left side:
        # Display an icon for the kind of cell, and its name on top of it.
        tile_y = 0 * (self.LINE_HEIGHT * 2 - data.TILE_HEIGHT) / 2
        self.list_surface.blit(
            data.tiles[
                {
                    TERRAIN_ILE:    'island',
                    TERRAIN_VOLCAN: 'volcano',
                    TERRAIN_MER:    'water',
                }[cell.type]
            ], (0, tile_y)
        )

        text = utils.make_bordered_text(
            game.CELL_TYPES[cell.type].capitalize(),
            self.font
        )
        text_w, text_h = text.get_size()
        self.list_surface.blit(
            text,
            (
                (data.TILE_WIDTH - text_w) / 2,
                tile_y + data.TILE_HEIGHT - text_h
            )
        )

        # First line:
        # Display position
        text = self.font.render(
            u'Position : (%d, %d)' % (x, y),
            True, utils.WHITE
        )
        self.list_surface.blit(
            text,
            (data.TILE_WIDTH + self.PADDING, 0)
        )

        second_line_y = text.get_size()[1] + self.PADDING

        if cell.player != game.NO_PLAYER:
            owner = self.game_state.players[cell.player].name
            color = data.get_player_color(self.game_state, cell.player)
        else:
            owner = 'Personne'
            color = utils.DARK_GREY

        text = self.font.render(owner, True, color)
        self.list_surface.blit(
            text, (data.TILE_WIDTH + self.PADDING, second_line_y)
        )

        # Second line:
        # Display gold and owner (only if ISLAND or VOLCANO).
        if cell.type in (TERRAIN_ILE, TERRAIN_VOLCAN):

            gold_icon_x = self.width - 2 * self.PADDING - data.ICON_WIDTH

            gold_pos = (gold_icon_x, second_line_y)
            self.list_surface.blit(data.icons['gold'], gold_pos)
            if not cell.gold:
                self.list_surface.blit(data.icon_shadow, gold_pos)

            text = self.font.render(
                u'%d' % cell.gold,
                False, utils.WHITE if cell.gold else utils.DARK_GREY
            )
            text_w, text_h = text.get_size()
            self.list_surface.blit(
                text,
                (
                    gold_icon_x - self.PADDING - text_w,
                    second_line_y + (data.ICON_HEIGHT - text_h) / 2
                )
            )

    def _display_player_stats(self, id, stats, entry_y):
        player_label = self.font.render(
            self.game_state.players[id].name,
            True, data.get_player_color(self.game_state, id)
        )
        if not any(stat for _, stat in stats):
            player_label.blit(self.line_shadow, (0, 0))
        self.list_surface.blit(
            player_label,
            (self.PADDING, entry_y)
        )

        left_shift = 2 * self.PADDING
        top_shift = entry_y + player_label.get_size()[1]
        remaining_width = self.width - left_shift - self.PADDING
        col_width = int(remaining_width / 3)
        text_space_w = col_width - data.ICON_WIDTH

        for i, (icon, stat) in enumerate(stats):
            col_left = left_shift + i * col_width
            # Icon for the stat
            self.list_surface.blit(data.icons[icon], (col_left, top_shift))
            if not stat:
                self.list_surface.blit(data.icon_shadow, (col_left, top_shift))
            # Numberic value
            text = self.font.render(
                str(stat), False,
                utils.WHITE if stat else utils.DARK_GREY
            )
            text_w, text_h = text.get_size()
            self.list_surface.blit(text, (
                col_left + data.ICON_WIDTH + (text_space_w - text_h) / 2,
                top_shift + (data.ICON_HEIGHT - text_h) / 2
            ))

    def _display_boat(self, boat, i):
        top_shift = i * self.LINE_HEIGHT
        left_shift = data.ICON_WIDTH + self.PADDING

        # Left side: display boat type illustration
        boat_pix = {
            BATEAU_CARAVELLE:   data.icons['caravelle'],
            BATEAU_GALION:      data.icons['galion'],
        }
        self.list_surface.blit(
            boat_pix[boat.type],
            (0, top_shift + (self.LINE_HEIGHT - data.ICON_HEIGHT) / 2)
        )

        # First line: display boat textual type
        text = self.font.render(
            game.BOAT_TYPES[boat.type].capitalize(),
            True, utils.WHITE
        )
        self.list_surface.blit(text, (left_shift, top_shift + self.PADDING))

        # Second line: display the boat owner
        text = self.font.render(
            self.game_state.players[boat.player].name,
            True, data.get_player_color(self.game_state, boat.player)
        )
        text_h = text.get_size()[1]
        self.list_surface.blit(
            text,
            (
                left_shift,
                top_shift + self.LINE_HEIGHT - text_h
            )
        )

        # Right side: display gold
        gold_icon_x = self.width - data.ICON_WIDTH - 2 * self.PADDING
        self.list_surface.blit(
            data.icons['gold'],
            (
                gold_icon_x,
                top_shift + (self.LINE_HEIGHT - data.ICON_HEIGHT) / 2
            )
        )
        text = self.font.render(str(boat.gold), True, utils.WHITE)
        text_w, text_h = text.get_size()
        self.list_surface.blit(
            text,
            (
                gold_icon_x - self.PADDING - text_w,
                top_shift + (self.LINE_HEIGHT - text_h) / 2
            )
        )

    def handle_click(self, x, y, but1, but2, but3):
        result = super(DetailsWidget, self).handle_click(
            x, y, but1, but2, but3
        )
        if result:
            return True
        # TODO: selecting a boat -> draw its way
        return False
        if not self.position:
            return False

        item = self.is_click_inside(x, y)
        if not item:
            return False
        if not but1:
            return True
        item -= 1
        units = self.game_state.map_units[self.position[1]][self.position[0]]
        if item < len(units):
            self.update_position(self.position[0], self.position[1], item)
        else:
            return False
