.PHONY: all clean help 

all:
	make -C src all
	cp src/takuzu .
clean:
	make -C src clean
	rm -f takuzu
help:
	make -C src help

