//
//  lda-inference.c
//  TopicEvo
//
//  Created by River on 12/8/15.
//  Copyright © 2015 River. All rights reserved.
//

#include "lda-inference.h"
#include <iostream>
#include <fstream>
using namespace std;
float VAR_CONVERGED=0.000001;
int VAR_MAX_ITER=30;


double lda_inference(document *doc,double *var_gamma, double **phi,
                     double **lamda, double **sigma,lda_model *model,int **topic_assign,
                     int **topic_assign_citation,
                     bool flag)
{
    double converged = 1;
    double phisum = 0, likelihood = 0,lamdasum=0,sigmasum=0;
    double likelihood_old = 0;
    int k, n, var_iter,i,j,l;
    double digamma_gam[model->num_topics_word],tmp1,tmp2,phisum_col,lamdasum_col;
    double test1=0;
    int test2=0;
    // compute posterior dirichlet
    
    ofstream fout;
//    fout.open("/Users/River/Documents/STUDY/Research/CODE/evolda3/out/test.txt");
    // intiation
    for (k = 0; k < model->num_topics_word; k++)
    {
        var_gamma[k] = model->alpha + ((doc->total+doc->cit_length)/((double) model->num_topics_word));
        digamma_gam[k] = digamma(var_gamma[k]);
        for (n = 0; n < doc->length; n++){
            phi[n][k] = 1.0/model->num_topics_word;
            //test=phi[n][k];
        }
        for (l=0; l<doc->cit_length; l++){
            lamda[l][k]=1.0/model->num_topics_word;
//            sigma[l][k]=1.0/model->num_topics;
        }
    }
    
    for (k=0; k<model->num_topics_citation; k++) {
        for (l=0; l<doc->cit_length; l++) {
            sigma[l][k]=1.0/model->num_topics_citation;
        }
    }
    var_iter = 0;
    
    while ((converged > VAR_CONVERGED) &&
           ((var_iter < VAR_MAX_ITER) || (VAR_MAX_ITER == -1)))
    {
        var_iter++;
//        if (var_iter%10==0) {
//            cout<<"inference iteration:"<<var_iter<<endl;
//            printf("[LDA INF] %8.5f %1.3e\n", likelihood, converged);
//        }
        
        //update phi
        for (n = 0; n < doc->length; n++)
        {
            phisum = 0;
            for (k = 0; k < model->num_topics_word; k++)
            {
                test1=digamma_gam[k];
                test2=doc->words[n];
                phi[n][k] =
                digamma_gam[k] +
                model->log_prob_w[k][doc->words[n]];      // phi is in log space temporally   ?????? ok
                
                if (k > 0)
                    phisum = log_sum(phisum, phi[n][k]);
                else
                    phisum = phi[n][k]; // note, phi is in log space
            }
            
            for (k=0; k<model->num_topics_word; k++) {
                phi[n][k]=exp(phi[n][k]-phisum);
            }
//            for (n=0; n<doc->length; n++) {
//                for (k=0; k<model->num_topics; k++) {
//                    fout<<phi[n][k]<<' ';
//                }
//                fout<<'\n';
//            }
            /*
             for (k = 0; k < model->num_topics; k++)
             {
             phi[n][k] = exp(phi[n][k] - phisum);        //phi is tranferred back
             var_gamma[k] =
             var_gamma[k] + doc->counts[n]*(phi[n][k] - oldphi[k]);              // ???????????approximate?
             // !!! a lot of extra digamma's here because of how we're computing it
             // !!! but its more automatically updated too.
             digamma_gam[k] = digamma(var_gamma[k]);
             }
             */
        }
        
       
        
        //update lamda and sigma
        for (l=0; l<doc->cit_length; l++) {
            lamdasum=0;
            sigmasum=0;
            for (i=0; i<model->num_topics_word; i++) {
                tmp1=0;
                for (j=0; j<model->num_topics_citation; j++) {
                    tmp1+=sigma[l][j]*model->log_zeta[i][j];
                }
                lamda[l][i]=digamma_gam[i]+tmp1;
                if (i>0)
                    lamdasum=log_sum(lamdasum,lamda[l][i]);     //log(exp(a)+exp(b))
                else
                    lamdasum=lamda[l][i];
            }
            
            for (i=0;i<model->num_topics_word; i++)
                lamda[l][i]=exp(lamda[l][i]-lamdasum);
            
            for (i=0;i<model->num_topics_citation;i++)
            {
                tmp2=0;
                for (j=0;j<model->num_topics_word;j++)
                {
                    tmp2+=lamda[l][j]*model->log_zeta[j][i];
                }
                sigma[l][i]=model->log_omega[doc->period]->log_omega_t[i][doc->citation[l]]+tmp2;
                if (i>0)
                    sigmasum=log_sum(sigmasum,sigma[l][i]);
                else
                    sigmasum=sigma[l][i];
            }
            
            
            for (i=0;i<model->num_topics_citation; i++)
                sigma[l][i]=exp(sigma[l][i]-sigmasum);
            
        }
        
        if(flag)
        {
            for (n = 0; n < doc->length; n++)
            {
//                if (doc->index<0 || doc->index>20988) {
//                    test1=1;
//                    cout<<"error1"<<endl;
//                }
                topic_assign[doc->index][n] =argmax(phi[n], model->num_topics_word);
//                test1=1;
            }
            
            for (l=0; l<doc->cit_length; l++) {
                topic_assign_citation[doc->index][l]=argmax(sigma[l],model->num_topics_citation);
            }
        }
        
        //update var_gamma
        for (i=0; i<model->num_topics_word; i++) {
            phisum_col=0;
            lamdasum_col=0;
            for (n=0; n<doc->length; n++)
                phisum_col+=doc->counts[n]*phi[n][i];
            for (l=0; l<doc->cit_length;l++)
                lamdasum_col+=lamda[l][i];
//            if (phisum_col==0 && lamdasum_col==0) {
//                cout<<"error"<<endl;
//            }
            var_gamma[i]=model->alpha+phisum_col+lamdasum_col;
            digamma_gam[i] = digamma(var_gamma[i]);
        }
        
        likelihood = compute_likelihood(doc, model, phi, var_gamma,sigma,lamda);
        assert(!isnan(likelihood));
        converged = (likelihood_old - likelihood) / likelihood_old;
        likelihood_old = likelihood;
        
        
    }
   
//    for (n=0; n<doc->length; n++) {
//        for (k=0; k<model->num_topics; k++) {
//            fout<<phi[n][k]<<' ';
//        }
//        fout<<'\n';
//    }
//    fout.close();
    //cout<<"Final Iteration Numbers"<<var_iter<<endl;
    
    return(likelihood);
    
}





double
compute_likelihood(document* doc, lda_model* model, double** phi, double* var_gamma,
                   double **sigma, double **lamda)
{
    double likelihood = 0, digsum = 0, var_gamma_sum = 0, dig[model->num_topics_word];
    int k, n,l,i,j;
    for (k = 0; k < model->num_topics_word; k++)
    {
        dig[k] = digamma(var_gamma[k]);
        var_gamma_sum += var_gamma[k];
    }
    digsum = digamma(var_gamma_sum);
    
    likelihood =
    lgamma(model->alpha * model -> num_topics_word)
    - model -> num_topics_word * lgamma(model->alpha)
    - (lgamma(var_gamma_sum));
    
    for (k = 0; k < model->num_topics_word; k++)
    {
        likelihood +=
        (model->alpha - 1)*(dig[k] - digsum) + lgamma(var_gamma[k])
        - (var_gamma[k] - 1)*(dig[k] - digsum);
        
        for (n = 0; n < doc->length; n++)
        {
            if (phi[n][k] > 0)                          //应该是不可能<=0的，记得监测一下
            {
                likelihood += doc->counts[n]*
                (phi[n][k]*((dig[k] - digsum) - log(phi[n][k])
                            + model->log_prob_w[k][doc->words[n]]));
            }
        }
    }
    
    for (l=0; l<doc->cit_length; l++) {
        for (i=0; i<model->num_topics_word; i++) {
            if (lamda[l][i]>0)
                likelihood+=lamda[l][i]*((dig[i]-digsum)-log(lamda[l][i]));
            
            
//            if  (sigma[l][i]>0)
//                likelihood+=+sigma[l][i]*(model->log_omega[doc->period]->log_omega_t[i][doc->citation[l]]
//                                          -log(sigma[l][i]));
            
            for (j=0; j<model->num_topics_citation; j++) {
                if (model->log_zeta[i][j]>0) {
                    likelihood+=sigma[l][j]*lamda[l][i]*log(model->log_zeta[i][j]);
                }
            }
        }
    }
    
    for (l=0; l<doc->cit_length; l++) {
        for (i=0; i<model->num_topics_citation; i++) {
            if  (sigma[l][i]>0)
                likelihood+=+sigma[l][i]*(model->log_omega[doc->period]->log_omega_t[i][doc->citation[l]]
                                          -log(sigma[l][i]));
        }
    }
    return(likelihood);
}