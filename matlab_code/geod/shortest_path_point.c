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
#include "mex.h"
#include "dijkstra.h"


void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[]){
const mxArray *mVertices, *mNeigh, *mSource;
double *temp;
double source;
double *dist;
double *path;
int n;

    /* Check number of input and output parameters */
    if (nrhs != 3){
        mexErrMsgTxt("Must have 3 input arguments");
    }
    if (nlhs != 2){
        mexErrMsgTxt("Must have 2 output arguments");
    }

    /* Get matlab inputs */
    mVertices = prhs[0];
    mNeigh = prhs[1];
    mSource = prhs[2]; 

    /* Get pointers to input data */
    temp = mxGetPr(mSource);
    source = (*temp);

    /* Get dimensions of data */
    n = mxGetM(mVertices);

    /* Create output data */
    plhs[0] = mxCreateDoubleMatrix(n, 1, mxREAL);
    plhs[1] = mxCreateDoubleMatrix(n, 1, mxREAL);

    /* Get pointers to output data */
    dist = mxGetPr(plhs[0]);
    path = mxGetPr(plhs[1]);

    /* Compute distance */
    dijkstra(mVertices, mNeigh, source, dist, path);
}


/* ******* */
/* * End * */
/* ******* */
