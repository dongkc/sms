#Maintainer: M0Rf30

pkgname=sms-git
pkgver=13.65fe4c8
pkgrel=1
pkgdesc="sending sms."
url="http://code.google.com/p/gyp/"
arch=('i686' 'x86_64' 'armv6h' 'armv7h')
makedepends=('git' 'cmake')
depends=('poco' 'google-glog' 'gflags' 'gammu')
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
  mkdir -p "${pkgdir}/usr/bin"
  mkdir -p "${pkgdir}/etc/rc.d"
  mkdir -p "${pkgdir}/usr/lib/systemd/system"

  install -m 755 smsd "${pkgdir}/usr/bin/smsd"
  install -m 755 gammurc "${pkgdir}/etc/gammurc"
  install -m 755 rc.local "${pkgdir}/etc/rc.d/rc.local"
  install -m 755 rc-local.service "${pkgdir}/usr/lib/systemd/system/rc-local.service"
}

md5sums=('SKIP')
