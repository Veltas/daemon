CFLAGS = -O2
daemon: daemon.c xarray.c
	gcc -std=c11 -Wall -pedantic -D_XOPEN_SOURCE $(CFLAGS) $^ -o $@
