all:
	gcc -Wall -g manifester.c -o manifester

clean:
	rm manifester

install: manifester
	cp manifester /usr/bin/

uninstall:
	rm /usr/bin/manifester
