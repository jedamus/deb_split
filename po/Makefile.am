EXTRA_DIST = LINGUAS POTFILES.in

EXTRA_DIST += @PACKAGE@.po @PACKAGE@.po.old

EXTRA_DIST += @PACKAGE@_de.po @PACKAGE@_de.po.old
EXTRA_DIST += de/LC_MESSAGES/@PACKAGE@.mo

EXTRA_DIST += @PACKAGE@_en.po @PACKAGE@_en.po.old
EXTRA_DIST += en/LC_MESSAGES/@PACKAGE@.mo

localedir = $(datadir)/locale

install-data-local:
		    $(MKDIR_P) $(localedir)/de/LC_MESSAGES
		    chmod u+rwx $(localedir)/de/LC_MESSAGES
		    $(INSTALL) -m 644 $(srcdir)/de/LC_MESSAGES/@PACKAGE@.mo $(localedir)/de/LC_MESSAGES
		    $(MKDIR_P) $(localedir)/en/LC_MESSAGES
		    chmod u+rwx $(localedir)/en/LC_MESSAGES
		    $(INSTALL) -m 644 $(srcdir)/en/LC_MESSAGES/@PACKAGE@.mo $(localedir)/en/LC_MESSAGES

uninstall-local:
		    rm -f $(localedir)/de/LC_MESSAGES/@PACKAGE@.mo
		    rm -f $(localedir)/en/LC_MESSAGES/@PACKAGE@.mo
