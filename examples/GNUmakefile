all: manpage.html

%.html: %.sl.html
	slhtml ./examples.sl.sh < $< > $@.tmp
	mv $@.tmp $@
