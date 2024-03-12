#!/usr/bin/env sh

# erzeugt Mittwoch, 20. November 2019 14:15 (C) 2019 von Leander Jedamus
# modifiziert Freitag, 08. März 2024 19:25 von Leander Jedamus
# modifiziert Dienstag, 27. Februar 2024 07:58 von Leander Jedamus
# modifiziert Montag, 02. Dezember 2019 02:53 von Leander Jedamus
# modifiziert Montag, 25. November 2019 10:09 von Leander Jedamus
# modifiziert Donnerstag, 21. November 2019 07:10 von Leander Jedamus
# modifiziert Mittwoch, 20. November 2019 14:16 von Leander Jedamus

set -e

autoreconf --install
gettextize -f
mv po/Makevars.template po/Makevars
./configure --prefix=/usr
make
make translate.sh
./translate.sh
# src/split
sleep 10
make distcheck

podir="po"
debiandir="debian"

if [ -d $podir ]; then
  PO_LINGUAS=$( if test -r $podir/LINGUAS; then grep -v "^\#" $podir/LINGUAS; fi)
  echo -n > $debiandir/install
  for lang in $PO_LINGUAS; do
    echo "po/$lang/LC_MESSAGES/split.mo /usr/share/locale/$lang/LC_MESSAGES" >> $debiandir/install
  done
fi

sudo dpkg-buildpackage -us -uc -nc -rfakeroot

# vim:ai sw=2

