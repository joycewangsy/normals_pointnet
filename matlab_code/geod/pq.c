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
/* * pq.c * */
/* ******** */


/* Includes */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pq.h"   
#include "config_pq.h"


/* Error handling */
#define ERRCHK_PREFIX pq
#define ERRCHK_VERSION PQ_VERSION
#define ERRCHK_INCLUDE_CODE
#define ERRCHK_COMPILE_LOW_LEVEL_ERROR_CHECK
#include <errchk.h>
#include <errchkconfig.h>
#include <errchkimp.h>


/* Defines */
/* Priority queue minimum size */
#define PQ_MIN_SIZE  1024


/* Macros */
#define pq_exch(rec1, rec2, temp, indx1, indx2)\
    do {\
        if ((rec1).indx && (rec2).indx){\
            (*((rec1).indx)) = (indx2);\
            (*((rec2).indx)) = (indx1);\
        }\
        memcpy(&(temp), &(rec1), sizeof(struct pq_rec));\
        memcpy(&(rec1), &(rec2), sizeof(struct pq_rec));\
        memcpy(&(rec2), &(temp), sizeof(struct pq_rec));\
    } while (0)

#define PQ_CHECK_POINTER(p, ret)\
    do {\
        if (!(p)){\
            errchk_set_error(errchk_assertion_failed,\
                    "invalid pointer (NULL pointer)");\
            ERRCHK_END;\
            return (ret);\
        }\
    } while (0)


/* Functions */
pq_t pq_create(void){ 
pq_t pq;

    ERRCHK_BEGIN(NULL);

    pq = (pq_t) malloc(sizeof(struct pq));
    if (!pq){
        errchk_set_error(errchk_memory,
                "could not allocate memory for pq struct");
        ERRCHK_END;
        return NULL;
    }

    pq->order = pq_less;
    pq->size = PQ_MIN_SIZE;
    pq->count = 0;
    pq->rec = (pq_rec_t) malloc(pq->size * sizeof(struct pq_rec));
    if (!pq->rec){
        free(pq);
        errchk_set_error(errchk_memory,
                "could not allocate memory for pq records");
        ERRCHK_END;
        return NULL;
    }

    ERRCHK_END;
    return pq;
}


int pq_destroy(pq_t pq){

    ERRCHK_BEGIN(0);

    PQ_CHECK_POINTER(pq, 0);

    if (pq){
        free(pq->rec);
        free(pq);
    }

    ERRCHK_END;
    return 1;
}
 

int pq_clear(pq_t pq){

    ERRCHK_BEGIN(0);

    PQ_CHECK_POINTER(pq, 0);
    
    pq->count = 0;

    ERRCHK_END;
    return 1;
}


int pq_reset(pq_t pq){
int size;
pq_rec_t rec;

    ERRCHK_BEGIN(0);
    
    PQ_CHECK_POINTER(pq, 0);
    
    size = PQ_MIN_SIZE;
    rec = (pq_rec_t) realloc(pq->rec, size * sizeof(struct pq_rec));
    if (!rec){
        errchk_set_error(errchk_memory,
                "could not reallocate memory for pq records");
        ERRCHK_END;
        return 0;
    }

    pq->count = 0;
    pq->size = size;
    pq->rec = rec;

    ERRCHK_END;
    return 1;
}


pq_t pq_dup(pq_t pq){
pq_t new;

    ERRCHK_BEGIN(NULL);
    
    PQ_CHECK_POINTER(pq, NULL);
    
    new = (pq_t) malloc(sizeof(struct pq));
    if (!new){
        errchk_set_error(errchk_memory,
                "could not allocate memory for pq struct");
        ERRCHK_END;
        return NULL;
    }

    new->order = pq->order;
    new->size = pq->size;
    new->count = pq->count;
    new->rec = (pq_rec_t) malloc(pq->size * sizeof(struct pq_rec));
    if (!new->rec){
        free(new);
        errchk_set_error(errchk_memory,
                "could not allocate memory for pq records");
        ERRCHK_END;
        return NULL;
    }
    
    memcpy(new->rec, pq->rec, pq->count * sizeof(struct pq_rec));

    ERRCHK_END;
    return new;
}


int pq_empty(pq_t pq){ 
int empty;

    ERRCHK_BEGIN(0);
    
    PQ_CHECK_POINTER(pq, 0);

    empty = (pq->count == 0);
    
    ERRCHK_END;
    return empty;
}


int pq_insert(pq_t pq, pq_key_t key, pq_data_t data, pq_indx_t *indx){
pq_rec_t temp;

    ERRCHK_BEGIN(0);
    
    PQ_CHECK_POINTER(pq, 0);

    if (pq->count == pq->size){
        pq->size *= 2;
        temp = (pq_rec_t) realloc(pq->rec,
                pq->size * sizeof(struct pq_rec));
        if (!temp){
            pq->size /= 2;
            errchk_set_error(errchk_memory,
                    "unable to reallocate pq record memory");
            ERRCHK_END;
            return 0;
        }
        pq->rec = temp;
    }

    pq->rec[pq->count].key = key;
    pq->rec[pq->count].data = data;
    pq->rec[pq->count].indx = indx;
    if (indx){
        (*pq->rec[pq->count].indx) = pq->count;
    }
    pq->count++;
    if (!pq_fixup(pq, pq->count-1)){
        errchk_cascade_error(errchk_memory,
                "unable to reorder queue");
        ERRCHK_END;
        return 0;
    }
    
    ERRCHK_END;
    return 1;
}


int pq_insert_rec(pq_t pq, pq_rec_t rec){
pq_rec_t temp;

    ERRCHK_BEGIN(0);
    
    PQ_CHECK_POINTER(pq, 0);
    PQ_CHECK_POINTER(rec, 0);
    
    if (pq->count == pq->size){
        pq->size *= 2;
        temp = (pq_rec_t) realloc(pq->rec,
                pq->size * sizeof(struct pq_rec));
        if (!temp){
            pq->size /= 2;
            errchk_set_error(errchk_memory,
                    "unable to reallocate pq memory");
            ERRCHK_END;
            return 0;
        }
        pq->rec = temp;
    }

    memcpy(&(pq->rec[pq->count]), &(rec), sizeof(struct pq_rec));
    if (pq->rec[pq->count].indx){
        (*pq->rec[pq->count].indx) = pq->count;
    }
    pq->count++;
    if (!pq_fixup(pq, pq->count-1)){
        errchk_cascade_error(errchk_memory,
                "unable to reorder queue");
        ERRCHK_END;
        return 0;
    }

    ERRCHK_END;
    return 1;
}


pq_data_t pq_delete(pq_t pq, pq_indx_t indx){ 
struct pq_rec temp;
pq_data_t data;

    ERRCHK_BEGIN(NULL);

    PQ_CHECK_POINTER(pq, NULL);

    if (pq->count > 0){
        pq_exch(pq->rec[indx], pq->rec[pq->count-1], temp,
                indx, pq->count-1);
        pq->count--;
        if ((indx > 0) &&
            (( (pq->order == pq_less) &&
               (pq->rec[(indx-1)/2].key < pq->rec[indx].key) ) ||
             ( (pq->order == pq_greater) &&
               (pq->rec[(indx-1)/2].key > pq->rec[indx].key) )) ){
            if (!pq_fixup(pq, indx)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        } else{
            if (!pq_fixdown(pq, indx)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        }
        if (pq->rec[pq->count].indx){
            (*pq->rec[pq->count].indx) = -1;
        }
        data = pq->rec[pq->count].data;
        ERRCHK_END;
        return data; 
    }

    errchk_set_error(errchk_assertion_failed, "pq is empty");
    ERRCHK_END;
    return NULL;
}


pq_rec_t pq_delete_rec(pq_t pq, pq_rec_t rec){ 
struct pq_rec temp;
pq_rec_t new;
int i;

    ERRCHK_BEGIN(NULL);

    PQ_CHECK_POINTER(pq, NULL);
    PQ_CHECK_POINTER(rec, NULL);

    if (pq->count > 0){
        new = (pq_rec_t) malloc(sizeof(struct pq_rec));
        if (!new){
            errchk_set_error(errchk_memory,
                    "could not allocate memory for pq record");
            ERRCHK_END;
            return NULL;
        }
     
        i = (*rec->indx);
        pq_exch(pq->rec[i], pq->rec[pq->count-1], temp, i, pq->count-1);
        pq->count--;
        if ((i > 0) &&
            (( (pq->order == pq_less) &&
               (pq->rec[(i-1)/2].key < pq->rec[i].key) ) ||
             ( (pq->order == pq_greater) &&
               (pq->rec[(i-1)/2].key > pq->rec[i].key) )) ){
            if (!pq_fixup(pq, i)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        } else{
            if (!pq_fixdown(pq, i)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        }
        if (pq->rec[pq->count].indx){
            (*pq->rec[pq->count].indx) = -1;
        }
        memcpy(&(new), &(pq->rec[pq->count]), sizeof(struct pq_rec));
        ERRCHK_END;
        return new;
    }

    errchk_set_error(errchk_assertion_failed, "pq is empty");
    ERRCHK_END;
    return NULL;
}


int pq_update(pq_t pq, pq_indx_t indx, pq_key_t key){

    ERRCHK_BEGIN(0);

    PQ_CHECK_POINTER(pq, 0);

    if (pq->count > 0){
        pq->rec[indx].key = key;
        if ((indx > 0) &&
            (( (pq->order == pq_less) &&
               (pq->rec[(indx-1)/2].key < pq->rec[indx].key) ) ||
             ( (pq->order == pq_greater) &&
               (pq->rec[(indx-1)/2].key > pq->rec[indx].key) )) ){
            if (!pq_fixup(pq, indx)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        } else{
            if (!pq_fixdown(pq, indx)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        }
        ERRCHK_END;
        return 1; 
    }

    errchk_set_error(errchk_assertion_failed, "pq is empty");
    ERRCHK_END;
    return 0;
}


int pq_update_rec(pq_t pq, pq_rec_t rec, pq_key_t key){
int indx;

    ERRCHK_BEGIN(NULL);

    PQ_CHECK_POINTER(pq, 0);
    PQ_CHECK_POINTER(rec, 0);

    if (pq->count > 0){
        indx = (*rec->indx);
        pq->rec[indx].key = key;
        if ((indx > 0) &&
            (( (pq->order == pq_less) &&
               (pq->rec[(indx-1)/2].key < pq->rec[indx].key) ) ||
             ( (pq->order == pq_greater) &&
               (pq->rec[(indx-1)/2].key > pq->rec[indx].key) )) ){
            if (!pq_fixup(pq, indx)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        } else{
            if (!pq_fixdown(pq, indx)){
                errchk_cascade_error(errchk_memory,
                        "unable to reorder queue");
                ERRCHK_END;
                return 0;
            }
        }
        ERRCHK_END;
        return 1; 
    }

    errchk_set_error(errchk_assertion_failed, "pq is empty");
    ERRCHK_END;
    return 0;
}


pq_data_t pq_delmax(pq_t pq){ 
struct pq_rec temp;
pq_data_t data;

    ERRCHK_BEGIN(NULL);

    PQ_CHECK_POINTER(pq, NULL);

    if (pq->count > 0){
        pq_exch(pq->rec[0], pq->rec[pq->count-1], temp, 0, pq->count-1);
        pq->count--;
        if (!pq_fixdown(pq, 0)){
            errchk_cascade_error(errchk_memory,
                    "unable to reorder queue");
            ERRCHK_END;
            return NULL;
        }
        if (pq->rec[pq->count].indx){
            (*pq->rec[pq->count].indx) = -1;
        }
        data = pq->rec[pq->count].data;
        ERRCHK_END;
        return data;
    }
    
    errchk_set_error(errchk_assertion_failed, "pq is empty");
    ERRCHK_END;
    return NULL;
}


pq_rec_t pq_delmax_rec(pq_t pq){ 
struct pq_rec temp;
pq_rec_t new;

    ERRCHK_BEGIN(NULL);

    PQ_CHECK_POINTER(pq, NULL);

    if (pq->count > 0){
        new = (pq_rec_t) malloc(sizeof(struct pq_rec));
        if (!new){
            errchk_set_error(errchk_memory,
                    "could not allocate memory for pq record");
            ERRCHK_END;
            return NULL;
        }
        
        pq_exch(pq->rec[0], pq->rec[pq->count-1], temp, 0, pq->count-1);
        pq->count--;
        if (!pq_fixdown(pq, 0)){
            errchk_cascade_error(errchk_memory,
                    "unable to reorder queue");
            free(new);
            ERRCHK_END;
            return NULL;
        }
        if (pq->rec[pq->count].indx){
            (*pq->rec[pq->count].indx) = -1;
        }
        memcpy(&(new), &(pq->rec[pq->count]), sizeof(struct pq_rec));
        ERRCHK_END;
        return new;
    }

    errchk_set_error(errchk_assertion_failed, "pq is empty");
    ERRCHK_END;
    return NULL;
}


int pq_fixup(pq_t pq, volatile pq_indx_t indx){
struct pq_rec temp;

    ERRCHK_BEGIN(0);

    PQ_CHECK_POINTER(pq, 0);

    while ((indx > 0) &&
            (( (pq->order == pq_less) &&
               (pq->rec[(indx-1)/2].key < pq->rec[indx].key) ) ||
             ( (pq->order == pq_greater) &&
               (pq->rec[(indx-1)/2].key > pq->rec[indx].key) )) ){
        pq_exch(pq->rec[indx], pq->rec[(indx-1)/2], temp, indx, (indx-1)/2);
        indx = (indx-1)/2;
    }

    ERRCHK_END;
    return 1;
}


int pq_fixdown(pq_t pq, volatile pq_indx_t indx){
struct pq_rec temp;
int j;

    ERRCHK_BEGIN(0);

    PQ_CHECK_POINTER(pq, 0);

    while ((2*indx+1) < pq->count){
        j = 2*indx+1;
        if ((j < (pq->count-1)) &&
                (( (pq->order == pq_less) &&
                  (pq->rec[j].key < pq->rec[j+1].key) ) ||
                 ( (pq->order == pq_greater) &&
                   (pq->rec[j].key > pq->rec[j+1].key) )) )
            j++;
        if (( (pq->order == pq_less) &&
              (pq->rec[indx].key >= pq->rec[j].key) ) ||
            ( (pq->order == pq_greater) &&
              (pq->rec[indx].key <= pq->rec[j].key) ))
            break;
        pq_exch(pq->rec[indx], pq->rec[j], temp, indx, j);
        indx = j;
     }

    ERRCHK_END;
    return 1;
}


/* ******* */
/* * End * */
/* ******* */
