# -*- Makefile -*-

lib_TARGETS = champion

# Tu peux rajouter des fichiers source ici
champion-dists = prologin.py

# Evite de toucher a ce qui suit
champion-dists += api.py interface.hh
champion-srcs = interface.cc

pc = $(shell which python2-config >/dev/null 2>&1 && echo python2-config || echo python-config)
champion-cxxflags = -fPIC $(shell $(pc) --includes)
champion-ldflags = -s $(shell $(pc) --ldflags)

include ../includes/rules.mk
