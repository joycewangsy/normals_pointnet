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
/* * shortest_path_st.c * */
/* ************************ */


#include <stdio.h>
#include <stdlib.h>
#include "mex.h"
#include "dijkstra.h"


void mexFunction(int nlhs, mxArray *plhs[],
        int nrhs, const mxArray *prhs[]){
const mxArray *mVertices, *mNeigh, *mSource, *mTarget;
double *temp;
double *temp2;
double source;
double target;
double *dist;
double *path;
double *succ;
int n;

    /* Check number of input and output parameters */
    if (nrhs != 4){
        mexErrMsgTxt("Must have 4 input arguments");
    }
    if (nlhs != 3){
        mexErrMsgTxt("Must have 3 output arguments");
    }

    /* Get matlab inputs */
    mVertices = prhs[0];
    mNeigh = prhs[1];
    mSource = prhs[2];
    mTarget = prhs[3];

    /* Get pointers to input data */
    temp = mxGetPr(mSource);
    source = (*temp);
    
    temp2 = mxGetPr(mTarget);
    target = (*temp2);

    /* Get dimensions of data */
    n = mxGetM(mVertices);

    /* Create output data */
    plhs[0] = mxCreateDoubleMatrix(n, 1, mxREAL);
    plhs[1] = mxCreateDoubleMatrix(n, 1, mxREAL);
    plhs[2] = mxCreateDoubleMatrix(1, 1, mxREAL);
    
    /* Get pointers to output data */
    dist = mxGetPr(plhs[0]);
    path = mxGetPr(plhs[1]);
    succ = mxGetPr(plhs[2]);
    
    /* Compute distance */
    dijkstra_st(mVertices, mNeigh, source, target, dist, path, succ);
    
}


/* ******* */
/* * End * */
/* ******* */
