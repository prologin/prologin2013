# -*- coding: utf-8 -*-

import pygame

import data
import utils
from widgets.base import BaseWidget

class HelpWidget(BaseWidget):
    WIDTH = 450
    HEIGHT = 375

    BORDER = 2
    PADDING = 16
    MARGIN = 96

    def __init__(self, screen_width, screen_height):
        super(HelpWidget, self).__init__(
            (screen_width - self.WIDTH) / 2,
            (screen_height - self.HEIGHT) / 2,
            self.WIDTH, self.HEIGHT
        )
        self.font = pygame.font.Font(data.get_font_path('font.ttf'), 13)
        self.update()

    def update(self):
        self.surface.fill(utils.WHITE)
        self.surface.fill(utils.BLACK, (
            self.BORDER, self.BORDER,
            self.width - 2 * self.BORDER,
            self.height - 2 * self.BORDER,
        ))

        vshift = self.PADDING
        descriptions = (
            ((u'H', ), u'Afficher/cacher l’aide'),
            None,
            ((u'Space', ), u'Activer/désactiver la pause'),
            ((u'N', ), u'Passer au tour suivant (en pause)'),
            None,
            ((u'Ctrl', u'↑', ), u'Monter dans la liste des unités'),
            ((u'Ctrl', u'↓', ), u'Descendre dans la liste des unités'),
        )

        key_x = self.PADDING
        caption_x = self.PADDING + self.MARGIN

        for description in descriptions:
            if description is None:
                vshift += self.PADDING
                continue
            keys, caption = description

            key_buttons = [utils.make_button(key, self.font) for key in keys]
            caption_text = self.font.render(caption, True, utils.WHITE)
            key_height = max(button.get_size()[1] for button in key_buttons)
            caption_height = caption_text.get_size()[1]

            line_height = max(key_height, caption_height)
            x = key_x
            for button in key_buttons:
                self.surface.blit(
                    button,
                    (x, vshift + (line_height - caption_height) / 2)
                )
                x += button.get_size()[0] + 3 * self.BORDER
            self.surface.blit(
                caption_text,
                (caption_x, vshift + (line_height - caption_height) / 2)
            )

            vshift += line_height + self.PADDING / 2
