PROJECT=c-slhtml
VERSION=1.0.0
PREFIX=/usr/local
PROGS=
CC=gcc -Wall -std=c99
all: slhtml$(EXE)
slhtml$(EXE): main.c slhtml.c slhtml.h GNUmakefile
	$(CC) -o $@ main.c slhtml.c
clean:
	rm -f slhtml$(EXE)
install:
	install -D -m 755 slhtml$(EXE) $(DESTDIR)$(PREFIX)/bin/slhtml$(EXE)
	install -D -m 755 slhtml-h     $(DESTDIR)$(PREFIX)/bin/slhtml-h
uninstall:
	rm -f $(DESTDIR)$(PREFIX)/bin/slhtml$(EXE)
	rm -f $(DESTDIR)$(PREFIX)/bin/slhtml$(EXE)
## -- BLOCK:c --
clean: clean-c
clean-c:
	rm -f *.o
## -- BLOCK:c --
## -- BLOCK:license --
install: install-license
install-license: 
	install -D -t $(DESTDIR)$(PREFIX)/share/doc/$(PROJECT) LICENSE
## -- BLOCK:license --
## -- BLOCK:man --
install: install-man
install-man:
	@mkdir -p $(DESTDIR)$(PREFIX)/share/man/man1
	cp ./slhtml.1 $(DESTDIR)$(PREFIX)/share/man/man1
## -- BLOCK:man --
