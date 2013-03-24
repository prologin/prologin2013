# -*- coding: utf-8 -*-

import pygame.transform

import data
import utils
from widgets.base import BaseWidget


class MinimapWidget(BaseWidget):

    def plug(self, widgets):
        self.map_widget = widgets['map']

    def update_game(self, game_state):
        self.map_surface = pygame.transform.smoothscale(
            self.map_widget.map_surface,
            (self.width, self.height)
        )
        great_map = self.map_widget.map_surface.get_size()
        self.ratio_x = float(self.width) / great_map[0]
        self.ratio_y = float(self.height) / great_map[1]
        self.update_view()

    def update_view(self):
        if not hasattr(self, 'map_surface'):
            return
        map = self.map_widget
        # Here, view is (x, y, w, h), in pixel coordinates.
        view = [
            map.position[0] * data.TILE_WIDTH,
            map.position[1] * (data.TILE_HEIGHT - data.TILE_OVERLAY),
            map.width, map.height
        ]

        # Here, we are computing (x_min, y_min, x_max, y_max)
        view[0] = utils.set_between(
            int(view[0] * self.ratio_x),
            lower=0, upper=self.width - 2
        )
        view[1] = utils.set_between(
            int(view[1] * self.ratio_y),
            lower=0, upper=self.height - 2
        )
        view[2] = utils.set_between(
            int(view[0] + view[2] * self.ratio_x),
            lower=2, upper=self.width
        )
        view[3] = utils.set_between(
            int(view[1] + view[3] * self.ratio_y),
            lower=2, upper=self.height
        )

        width, height = view[2] - view[0], view[3] - view[1]
        frame = utils.make_surface(width, height)
        frame.fill(utils.BLACK, (1, 1, width - 1, height - 1))
        frame.fill(utils.WHITE, (0, 0, width - 1, height - 1))
        frame.fill(utils.BLACK, (1, 1, width - 3, 1))
        frame.fill(utils.BLACK, (1, 1, 1, height - 3))
        frame.fill((0, 0, 0, 0), (2, 2, width - 4, height - 4))
        self.surface.blit(self.map_surface, (0, 0))
        self.surface.blit(frame, (view[0], view[1]))

    def handle_click(self, x, y, *buts):
        coords = self.is_click_inside(x, y)
        if not coords:
            return False
        self.map_widget.handle_view_click(
            int(coords[0] / self.ratio_x),
            int(coords[1] / self.ratio_y),
            False, False, True,
            absolute=True
        )
        return True
