SUBDIRS = lib src po
dist_doc_DATA = README README.md
# DIST_SOURCES = run_me_once.sh translate.sh.in
EXTRA_DIST = run_me_once.sh translate.sh.in translate.sh

do_subst = sed -e 's,[@]datadir[@],$(datadir),g' \
               -e 's,[@]PERL[@],$(PERL),g'       \
	       -e 's,[@]PACKAGE[@],$(PACKAGE),g' \
	       -e 's,[@]VERSION[@],$(VERSION),g'

translate.sh: translate.sh.in
	      $(do_subst) < translate.sh.in > translate.sh
	      chmod +x translate.sh

clean-local:
	    @rm config.status configure config.log config.h.in config.h config.guess config.sub
	    @rm Makefile ABOUT-NLS 
	    @rm -r autom4te.cache m4
	    @rm aclocal.m4
	    @rm compile install-sh missing Makefile.in
	    @mv Makefile.am~ Makefile.am
	    @rm Makefile.in~ 
	    @rm ar-lib depcomp config.rpath 
	    @rm ChangeLog ChangeLog~ stamp-h1
	    @rm translate.sh
