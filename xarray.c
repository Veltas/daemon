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

#include "xarray.h"
#include <stdlib.h>

/* Data header that goes in the front of the block, followed by array */
struct Data
{
  size_t element_size, size, capacity;
};

/* Gives block pointer from array pointer */
static void *array_to_block(void *const array)
{
  return (char *)array - sizeof (struct Data);
}
static const void *array_to_block_c(const void *const array)
{
  return (const char *)array - sizeof (struct Data);
}

/* Gives array pointer from block pointer */
static void *block_to_array(void *const block_p)
{
  return (char *)block_p + sizeof (struct Data);
}

void *xa_alloc(
  const size_t element_size,
  const size_t initial_size
)
{
  /* Choose capacity */
  const size_t initial_capacity = initial_size > 0 ? initial_size : 1;

  /* Allocate block */
  const size_t block_size =
    sizeof (struct Data) + element_size * initial_capacity;
  void *const block_p = malloc(block_size);

  /* Initialise data */
  struct Data *const data = (struct Data *)block_p;
  data->element_size = element_size;
  data->size = initial_size;
  data->capacity = initial_capacity;

  /* Return array */
  return block_to_array(block_p);
}

void xa_free(
  void *const xarray
)
{
  if (xarray != NULL)
  {
    free(array_to_block(xarray));
  }
}

void xa_extend(
  void *const xarray_p,
  const size_t extend_amount
)
{
  /* Check capacity */
  void *block_p = array_to_block(*(void **)xarray_p);
  struct Data *data = block_p;

  if (data->capacity < data->size + extend_amount)
  {
    /* Increase size of array */
    size_t target_size;
    if (data->capacity * 2 >= data->size + extend_amount)
    {
      target_size =
	sizeof (struct Data) +
	data->element_size * data->capacity * 2;
    }
    else
    {
      target_size =
	sizeof (struct Data) +
	(data->size + extend_amount) * data->element_size;
    }
    block_p = realloc(block_p, target_size);

    /* Update values */
    *(void **)xarray_p = block_to_array(block_p);
    data = block_p;
    data->capacity =
      (target_size - sizeof (struct Data)) / data->element_size;
  }
  data->size += extend_amount;
}

size_t xa_size(
  const void *const xarray
)
{
  const struct Data *const data = array_to_block_c(xarray);
  return data->size;
}
