# -*- coding: iso-8859-1 -*-
# This file has been generated, if you wish to
# modify it in a permanent way, please refer
# to the script file : gen/generator_python.rb

import threading

import pygame

from api import *
import settings
import state_reader
from window import Window

try:
    import psyco
    psyco.full()
except:
    pass

settings.load(['champion.so'])

state_reader = state_reader.StechecReader()

def run_pygame():
    pygame.init()
    window = Window(state_reader)
    window.run()

def partie_init():
    gfx_thread = threading.Thread(target=run_pygame)
    gfx_thread.start()

def jouer_tour():
    state_reader.put_state()

def partie_fin():
    state_reader.do_end()
