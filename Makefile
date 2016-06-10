#This file is part of daemon.
#
#daemon is free software: you can redistribute it and/or modify
#it under the terms of the GNU General Public License as published by
#the Free Software Foundation, either version 3 of the License, or
#(at your option) any later version.
#
#Foobar is distributed in the hope that it will be useful,
#but WITHOUT ANY WARRANTY; without even the implied warranty of
#MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#GNU General Public License for more details.
#
#You should have received a copy of the GNU General Public License
#along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

CSTD = c90
CWARN = -Wall -W -pedantic

CFLAGS = -std=$(CSTD) $(CWARN) -D_XOPEN_SOURCE -O2
prefix = /usr/local

daemon: xarray.o

.PHONY: install
install: daemon
	mkdir -p $(prefix)/bin
	install daemon $(prefix)/bin

.PHONY: clean
clean:
	rm -f daemon *.o
