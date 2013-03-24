# -*- coding: iso-8859-1 -*-
# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

import threading

import pygame
pygame.init()

from api import *
import settings
import state_reader
import window

try:
    import psyco
    psyco.full()
except:
    pass

settings.load(['champion.so'])

state_reader = state_reader.StechecReader()
window = window.Window(state_reader)
gfx_thread = threading.Thread(target=window.run)

def partie_init():
    gfx_thread.start()

def jouer_tour():
    state_reader.put_state()

def partie_fin():
    state_reader.do_end()
