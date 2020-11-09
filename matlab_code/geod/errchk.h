/*
 *  errchk - generic error check system
 *  Copyright (c) 2005-2010
 *      Oliver Matias van Kaick <ovankaic@gmail.com>
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 *  See the GNU General Public License for more details (file COPYING).
 */


/* ************ */
/* * errchk.h * */
/* ************ */

#ifndef __ERRCHK_H__
#define __ERRCHK_H__


/* Generic error types */
typedef enum {
    errchk_none             =  0,
    errchk_unknown          =  1,
    errchk_sigsegv          =  2,
    errchk_memory           =  3,
    errchk_io               =  4,
    errchk_assertion_failed =  5,
    errchk_invalid_index    =  6,
    errchk_invalid_domain   =  7,
    errchk_invalid_argument =  8,
    errchk_invalid_range    =  9,
    errchk_logic            = 10,
    errchk_overflow         = 11,
    errchk_underflow        = 12,
    errchk_division_by_zero = 13,
    errchk_other            = 14,
} errchk_t;


/* Generic user error function type */
typedef void (*errchk_func_t)(errchk_t errcode, char *error);


#endif /* __ERRCHK_H__ */

/* ******* */
/* * End * */
/* ******* */
