//
//  lda-alpha.cpp
//  evolda
//
//  Created by huangxin on 15/12/13.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#include <stdio.h>
#include "lda-alpha.h"

/*
 * objective function and its derivatives
 *
 */

double alhood(double a, double ss, int D, int K)
{ return(D * (lgamma(K * a) - K * lgamma(a)) + (a - 1) * ss); }

double d_alhood(double a, double ss, int D, int K)
{ return(D * (K * digamma(K * a) - K * digamma(a)) + ss); }

double d2_alhood(double a, int D, int K)
{ return(D * (K * K * trigamma(K * a) - K * trigamma(a))); }


/*
 * newtons method
 *
 */

double opt_alpha(double ss, int D, int K)
{
    double a, log_a, init_a = 100;
    double f, df, d2f;
    int iter = 0;
    
    log_a = log(init_a);
    do
    {
        
        iter++;
        a = exp(log_a);
        if (isnan(a))
        {
            init_a = init_a * 10;
            printf("warning : alpha is nan; new init = %5.5f\n", init_a);
            a = init_a;
            log_a = log(a);
        }
        f = alhood(a, ss, D, K);
        df = d_alhood(a, ss, D, K);         // digmma includes log, log(a) returns nan when a<0
        d2f = d2_alhood(a, D, K);
        log_a = log_a - df/(d2f * a + df);              //derivation with respect to log_a, it's correct
//        printf("alpha maximization : %5.5f   %5.5f\n", f, df);
    }
    while ((fabs(df) > NEWTON_THRESH) && (iter < MAX_ALPHA_ITER));
    return(exp(log_a));
}
