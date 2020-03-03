#!/usr/bin/env python3
# SPDX-License-Identifier: GPL-2.0-or-later
# Copyright (c) 2012-2020 Association Prologin <association@prologin.org>

import glob
import os.path

from wafgenerator import generator_player_install


def options(opt):
    pass


def configure(cfg):
    pass


def build(bld):
    bld.shlib(
        source='''
            src/action-ack.cc
            src/action-charge.cc
            src/action-colonize.cc
            src/action-construct.cc
            src/action-discharge.cc
            src/action-move.cc
            src/action-transfer.cc
            src/api.cc
            src/cell.cc
            src/dumper.cc
            src/entry.cc
            src/game.cc
            src/interface.cc
            src/map.cc
            src/rules.cc
            src/tools.cc
        ''',
        defines=['MODULE_COLOR=ANSI_COL_BROWN', 'MODULE_NAME="rules"'],
        target='prologin2013',
        use=['stechec2'],
    )

    abs_pattern = os.path.join(bld.path.abspath(), 'src/tests/test-*.cc')
    for test_src in glob.glob(abs_pattern):

        test_name = os.path.split(test_src)[-1]
        test_name = test_name[len("test-"):-(len(".cc"))]

        # Waf requires a relative path for the source
        src_relpath = os.path.relpath(test_src, bld.path.abspath())

        bld.program(
            features='gtest',
            source=src_relpath,
            target='prologin2013-test-{}'.format(test_name),
            use=['prologin2013', 'stechec2-utils'],
            includes=['.'],
            defines=['MODULE_COLOR=ANSI_COL_PURPLE',
                     'MODULE_NAME="prologin2013"'],
        )

    bld.install_files('${PREFIX}/share/stechec2/prologin2013', [
        'prologin2013.yml',
    ])

    generator_player_install(bld, 'prologin2013')
