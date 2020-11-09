/*
 *  shortest_path_face - shortest path distances from a source face
 *  Copyright (c) 2009-2011
 *      Oliver van Kaick <ovankaic@gmail.com>
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


/* ************************ */
/* * shortest_path_face.c * */
/* ************************ */


#include <stdio.h>
#include <stdlib.h>
#include "pq.h"
#include <math.h>
#include "mex.h"
#include "dijkstra.h"


extern char pq_error[];


void dijkstra(const mxArray *vertices, const mxArray *mNeigh, double source, double *dist, double *path){
double n;
int i;
pq_t pq;
int temp;
int *indx;
mxArray *neigh;
double *inc;
int count;
double edge_len;
double center1[3];
double center2[3];

    /* Get number of points */
    n = mxGetM(vertices);

    /* Reset distance vector to infinity */
    for (i = 0; i < n; i++){
        dist[i] = HUGE;
    }

    /* Initialize path (predecessor array) */
    for (i = 0; i < n; i++){
        path[i] = -1;
    }

    /* Allocate memory for priority queue back links */
    indx = (int *) malloc(n * sizeof(int));
    if (!indx){
        mexErrMsgTxt(
                "could not allocate memory for heap back links");
        return;
    }
    for (i = 0; i < n; i++){
        indx[i] = -1;
    }

    /* Create priority queue */
    pq = pq_create();
    if (!pq){
        mexErrMsgTxt(
                "could not create priority queue");
        free(indx);
        return;
    }

    /* Set ordering of priority queue */
    pq->order = pq_greater;

    /* Set distance from source to source as 0 */
    dist[(int) (source-1.0)] = 0;

    /* Set predecessor of the source as itself */
    path[(int) (source-1.0)] = source;

    /* Add source to the priority queue */
    pq_insert(pq, 0, (int) source, &(indx[(int) (source-1.0)]));

    /* Dijkstra's algorithm */
    while (!pq_empty(pq)){
        /* Get element with smallest distance */
        temp = pq_delmax(pq);
        /* Check if element makes sense */
        if (dist[temp-1] != HUGE){
            /* Get neighbors */
            neigh = mxGetCell(mNeigh, temp-1);
            inc = mxGetPr(neigh);
            count = mxGetNumberOfElements(neigh);
            /* Loop through neighbors */
            for (i = 0; i < count; i++){
                /* Get edge length: distance between face centroids */
                point_coord(vertices, temp, center1);
                point_coord(vertices, inc[i], center2);
                edge_len = edge_length(center1, center2);
                /* Check if neighbor is in the queue or not */
                if (dist[(int) (inc[i]-1.0)] == HUGE){
                    /* If not in the queue, update its distance and add
                     * it to the queue */
                    /* Update distance */
                    dist[(int) (inc[i]-1.0)] = dist[temp-1] + edge_len;
                    /* Set predecessor in the path  */
                    path[(int) (inc[i]-1.0)] = temp;
                    /* Check if element is really not in the queue */
                    if (indx[(int) (inc[i]-1.0)] != -1){
                        mexErrMsgTxt("inconsistency in queue management (1)");
                    }
                    /* Add it to the queue */
                    pq_insert(pq, dist[(int) (inc[i]-1.0)],
                            inc[i], &(indx[(int) (inc[i]-1.0)]));
                } else {
                    /* If already in the queue, update its distance and
                     * key only if it is better through the current node */
                    if (dist[(int) (inc[i]-1.0)] > (dist[temp-1] + edge_len)){
                        dist[(int) (inc[i]-1.0)] = dist[temp-1] + edge_len;
                        path[(int) (inc[i]-1.0)] = temp;
                        /* Check if element is really in the queue */
                        if (indx[(int)(inc[i]-1.0)] == -1){
                            mexErrMsgTxt("inconsistency in queue management (2)");
                        }
                        /* Update key of element in the queue */
                        pq_update(pq, indx[(int) (inc[i]-1.0)],
                                        dist[(int) (inc[i]-1.0)]);
                    }
                }
            }
        } else {
            mexErrMsgTxt("unreachable vertex reached");
        }
    }

    /* Destroy priority queue */
    pq_destroy(pq);

    /* Free memory */
    free(indx);
}


void point_coord(const mxArray *vertices, int vertex, double center[3]){
double *coord;
int j, n;

    coord = mxGetPr(vertices);
    n = mxGetM(vertices);
    for (j = 0; j < 3; j++){
        center[j] = coord[el_indx(vertex-1, j, n)];
    }
}


double edge_length(double center1[3], double center2[3]){
double diff[3];
double len;

    diff[0] = center1[0] - center2[0];
    diff[1] = center1[1] - center2[1];
    diff[2] = center1[2] - center2[2];
    len = sqrt(diff[0]*diff[0] + diff[1]*diff[1] + diff[2]*diff[2]);
    return len;
}