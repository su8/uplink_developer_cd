prefix:=$(PWD)/install

config:
	cd gltt-2.5 ; rm -f config.cache ; ./configure --prefix=$(prefix)
	cd libmikmod-3.1.9 ; rm -f config.cache ; ./configure --prefix=$(prefix)

all:
	$(MAKE) -C gltt-2.5
	$(MAKE) -C gltt-2.5 install
	$(MAKE) -C libmikmod-3.1.9
	$(MAKE) -C libmikmod-3.1.9 install
	cd tcp4u.330/src ; make -f ../Unix/Makefile

clean:
	$(MAKE) -C gltt-2.5 clean
	$(MAKE) -C libmikmod-3.1.9 clean
	cd tcp4u.330/src ; make -f ../Unix/Makefile clean
