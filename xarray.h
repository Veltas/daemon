/*
This file is part of daemon.

daemon is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

Foobar is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with Foobar.  If not, see <http://www.gnu.org/licenses/>.
*/

// xarray is an extendable array, it handles managing size/capacity
// seperately automatically. It manages allocated arrays as pointers
// to the arrays themselves, which makes using this in C especially
// easy.

#ifndef XARRAY_H
#define XARRAY_H

#include <stddef.h>

// Allocates new array with a given initial size.
// On allocation error terminates program with error message.
void *xa_alloc(
  size_t element_size,
  size_t initial_size
);

// Frees resources for a previously allocated array. If given NULL, no
// action is taken. Given one of these two, provided the heap is not
// corrupted, this function will not fail.
void xa_free(
  void *xarray
);

// Extends the array, given a pointer to where the array pointer is
// stored. May change the array pointer as part of reallocating array
// space. Failure results in termination of program.
void xa_extend(
  void **xarray_p,
  size_t extend_amount
);

// Returns the current size (not capacity) of an xarray.
size_t xa_size(
  const void *xarray
);

#endif
