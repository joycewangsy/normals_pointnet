/*
 *  pq - generic priority queue
 *  Copyright (c) 2003-2010
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


/* ******** */
/* * pq.h * */
/* ******** */

#ifndef __PQ_H__
#define __PQ_H__


#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */


/* Error handling (variables defined by errchk library) */
/* Error number */
/* extern errchk_t io_errcode; */
/* Error string */
/* extern char io_error[]; */
/* User error function type */
/* extern errchk_func_t io_error_func; */
/* Compiled with low-level error check possibility */
/* char io_low_level_error_check_support; */
/* Check low-level errors */
/* char io_low_level_error_check; */
/* Library version */
/* char *io_version; */


/* Types */
/* Priority queue ordering */
typedef enum {
    pq_less = 0,
    pq_greater = 1,
} pq_order_t;
/* Key type */
typedef double pq_key_t;
/* Array indexing and item count */
typedef int pq_indx_t;
/* pq_indx_t has to be signed */
/* Data type */
/*typedef void *pq_data_t;*/
typedef int pq_data_t;

/* Record */
typedef struct pq_rec {
    pq_key_t key;
    pq_indx_t *indx;
    pq_data_t data;
} *pq_rec_t;

/* Priority queue */
typedef struct pq {
    pq_order_t order;
    pq_indx_t count;
    pq_indx_t size;
    pq_rec_t rec;
} *pq_t;


/* Functions */
pq_t pq_create(void);
int pq_destroy(pq_t pq);
int pq_clear(pq_t pq);
int pq_reset(pq_t pq);
pq_t pq_dup(pq_t pq);

int pq_empty(pq_t pq);
int pq_insert(pq_t pq, pq_key_t key, pq_data_t data, pq_indx_t *indx);
int pq_insert_rec(pq_t pq, pq_rec_t rec);
pq_data_t pq_delete(pq_t pq, pq_indx_t indx);
pq_rec_t pq_delete_rec(pq_t pq, pq_rec_t rec);
pq_data_t pq_delmax(pq_t pq);
pq_rec_t pq_delmax_rec(pq_t pq);
int pq_update(pq_t pq, pq_indx_t indx, pq_key_t key);
int pq_update_rec(pq_t pq, pq_rec_t rec, pq_key_t key);

int pq_fixup(pq_t pq, pq_indx_t indx);
int pq_fixdown(pq_t pq, pq_indx_t indx);


#ifdef __cplusplus
}
#endif /* __cplusplus */


#endif /* __PQ_H__ */

/* ******* */
/* * End * */
/* ******* */
