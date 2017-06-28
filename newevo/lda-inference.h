//
//  lda-inference.h
//  TopicEvo
//
//  Created by River on 12/8/15.
//  Copyright © 2015 River. All rights reserved.
//

#ifndef lda_inference_h
#define lda_inference_h

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <assert.h>
#include "utils.h"
#include "dataset.h"

extern float VAR_CONVERGED;
extern int VAR_MAX_ITER;

double lda_inference(document *doc,double *var_gamma, double **phi,
                     double **lamda, double **sigma,lda_model *model,
                     int **topic_assign,int **topic_assign_citation,bool flag);

double compute_likelihood(document* doc, lda_model* model, double** phi, double* var_gamma,
                          double **sigma, double **lamda);

#endif /* lda_inference_h */
