/* libunwind - a platform-independent unwind library
   Copyright (C) 2008 CodeSourcery
   Copyright (C) 2014 Tilera Corp.

This file is part of libunwind.

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
"Software"), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.  */

#include <stdlib.h>

#include "unwind_i.h"

PROTECTED unw_addr_space_t
unw_create_addr_space (unw_accessors_t *a, int byte_order)
{
#ifdef UNW_LOCAL_ONLY
  return NULL;
#else
  unw_addr_space_t as = malloc (sizeof (*as));

  if (!as)
    return NULL;

  memset (as, 0, sizeof (*as));

  as->acc = *a;

  /*
   * Tilegx supports only big or little-endian, not weird stuff like
   * PDP_ENDIAN.
   */
  if (byte_order != 0
      && byte_order != __LITTLE_ENDIAN
      && byte_order != __BIG_ENDIAN)
    return NULL;

  if (byte_order == 0)
    /* use host default: */
    as->big_endian = (__BYTE_ORDER == __BIG_ENDIAN);
  else
    as->big_endian = (byte_order == __BIG_ENDIAN);

  as->abi = UNW_TILEGX_ABI_N64;
  as->addr_size = 8;

  return as;
#endif
}
