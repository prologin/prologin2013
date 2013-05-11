# Maintainer: Prologin <serveur@prologin.org>
# Contributor: Prologin <serveur@prologin.org>
pkgname=stechec2-prologin2013
pkgver=2013_0.1
pkgrel=1
pkgdesc="Stechec2 with Prologin 2013 rules"
arch=('x86' 'x86_64')
url="http://prologin.org/"
license=('GPL')
provides=('stechec2' 'prologin2013')
depends=('google-gflags' 'python2' 'zeromq')
makedepends=('git')

_gitroot_stechec2="https://bitbucket.org/prologin/stechec2"
_gitroot_prologin2013="https://bitbucket.org/prologin/prologin2013"

build() {
    cd $srcdir
    if [ -d stechec2 ]; then
        cd stechec2
        git pull
    else
        git clone $_gitroot_stechec2
        cd stechec2
    fi

    if [ -d games/prologin2013 ]; then
        cd games/prologin2013
        git pull
        cd ../..
    else
        git clone $_gitroot_prologin2013 games/prologin2013
    fi

    python2 waf.py configure build --prefix=/usr      \
        --with-games=prologin2013 --checkall || return 1
}

package() {
    cd $srcdir/stechec2
    python2 waf.py install --destdir=$pkgdir
    rm -rf build
}