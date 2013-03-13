#! /usr/bin/env python2

def options(opt):
    pass

def configure(cfg):
    pass

def build(bld):
    bld.shlib(
        source = '''
            src/rules.cc
            src/entry.cc
            src/api.cc
            src/interface.cc
            src/cell.cc
            src/map.cc
            src/game.cc
        ''',
        target = 'prologin2013',
        use = ['net', 'rules', 'utils'],
        defines = ['MODULE_COLOR=ANSI_COL_PURPLE',
            'MODULE_NAME="prologin2013"'],
        lib = ['dl'],
    )

    for test in []:
        bld.program(
            features = 'gtest',
            source = 'src/tests/test-%s.cc' % test,
            target = 'prologin2013-test-%s' % test,
            use = ['prologin2013', 'utils'],
            includes = ['.'],
            defines = ['MODULE_COLOR=ANSI_COL_PURPLE',
                'MODULE_NAME="prologin2013"'],
        )

    bld.install_files('${PREFIX}/share/stechec2/prologin2013', [
        'prologin2013.yml',
    ])

