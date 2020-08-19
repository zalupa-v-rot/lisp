NAME?=
REALDIR?=
BUILDDIR?=
LIBRARY=$(BUILDDIR)/lib/libsimple.a


$(NAME): $(LIBRARY) $(REALDIR)/$(NAME)
	$(CC) $(CFLAGS) -o ./$@ $(REALDIR)/$@/*.c $(LIBRARY)
	./$@

$(LIBRARY):
	mkdir -p $(BUILDDIR)/simplelib
	cd $(BUILDDIR)/simplelib && $(REALDIR)/../configure CC="$(CC)" CFLAGS="$(CFLAGS)" --prefix="$(BUILDDIR)"
	cd $(BUILDDIR)/simplelib && make install

_FORCE:
