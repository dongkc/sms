#Maintainer: M0Rf30

pkgname=sms-git
pkgver=11.8a94724
pkgrel=1
pkgdesc="sending sms."
url="http://code.google.com/p/gyp/"
arch=('i686' 'x86_64' 'armv6h' 'armv7h')
makedepends=('git' 'cmake')
depends=('poco' 'google-glog' 'gflags')
license=('custom')
source=("git+https://github.com/dongkc/sms")
conflicts=('sms')
replaces=('sms')

pkgver() {
  cd sms
  echo $(git rev-list --count master).$(git rev-parse --short master)
}

build()
{
  cd sms
  cmake . && make
}

package() {
  cd sms
  mkdir -p "${pkgdir}/usr/bin/"
  mkdir -p "${pkgdir}/etc/"
  install -m 755 smsd "${pkgdir}/usr/bin/smsd"
  install -m 755 gammurc "${pkgdir}/gammurc"
}

md5sums=('SKIP')
