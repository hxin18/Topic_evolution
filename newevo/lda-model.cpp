//
//  lda-model.cpp
//  evolda
//
//  Created by huangxin on 15/12/13.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#include <stdio.h>
#include "lda-model.h"
#include <iostream>
/*
 * compute MLE lda model from sufficient statistics
 *
 */


void lda_mle(lda_model* model, lda_suffstats* ss, int estimate_alpha)
{
    int k;
    int w;
    
    for (k = 0; k < model->num_topics_word; k++)
    {
        for (w = 0; w < model->num_terms; w++)
        {
            if (ss->class_word[k][w] > 0)
            {
                model->log_prob_w[k][w] =
                log(ss->class_word[k][w]) -
                log(ss->class_total[k]);
            }
            else
                model->log_prob_w[k][w] = -100;
        }
    }
    
    //newly added: update Zeta(topic-topic)
    int t1, t2;
    for (t1 = 0; t1 < model->num_topics_word; ++t1)
    {
        for (t2 = 0; t2 < model->num_topics_citation; ++t2)
        {
            if (ss->class_topic[t1][t2] > 0)
            {
                model->log_zeta[t1][t2] =
                log(ss->class_topic[t1][t2]) -
                log(ss->class_topic_total[t1]);
            }
            else
            {
                model->log_zeta[t1][t2] = -100;
            }
        }
    }
    //
    
    //newly added: update Omega(topic-doc)
    int period, t, d;
    for (period = 0; period < model->num_periods; ++period)
    {
        for (t = 0; t < model->num_topics_citation; ++t)
        {
            for (d = 0; d < model->docs_per_period[period]; ++d)
            {
                if ( ss->class_omega[period]->class_doc[t][d] > 0)
                {
                    model->log_omega[period]->log_omega_t[t][d] =
                    log(ss->class_omega[period]->class_doc[t][d]) -
                    log(ss->class_omega[period]->class_doc_total[t]);
                }
                else
                    model->log_omega[period]->log_omega_t[t][d] = -100;
            }
        }
    }
    //
    
    
    if (estimate_alpha == 1)
    {
        model->alpha = opt_alpha(ss->alpha_suffstats,
                                 ss->num_docs,
                                 model->num_topics_word);
        
        printf("new alpha = %5.5f\n", model->alpha);
    }
}
///*
// * allocate sufficient statistics
// *
// */
//

lda_suffstats* new_lda_suffstats(lda_model* model)
{
    int num_topics_words = model->num_topics_word;
    int num_topics_citation=model->num_topics_citation;
    int num_terms = model->num_terms;
    int num_periods = model->num_periods;
    int i,j,k;
    
    lda_suffstats* ss = new lda_suffstats;
    //malloc(sizeof(lda_suffstats));
    
    ss->class_total = new double[num_topics_words];
    //malloc(sizeof(double)*num_topics);
    ss->class_word = new double*[num_topics_words];
    //malloc(sizeof(double*)*num_topics);
    for (i = 0; i < num_topics_words; i++)
    {
        ss->class_total[i] = 0;
        ss->class_word[i] = new double[num_terms];
        //malloc(sizeof(double)*num_terms);
        for (j = 0; j < num_terms; j++)
        {
            ss->class_word[i][j] = 0;
        }
    }
    
    ss->class_topic_total = new double[num_topics_words];
    //malloc(sizeof(double)*num_topics);
    ss->class_topic = new double*[num_topics_words];
    //malloc(sizeof(double*)*num_topics);
    for (i = 0; i < num_topics_words; i++)
    {
        ss->class_topic_total[i] = 0;
        ss->class_topic[i] = new double[num_topics_citation];
        //malloc(sizeof(double)*num_topics);
        for (j = 0; j < num_topics_citation; j++)
        {
            ss->class_topic[i][j] = 0;
        }
    }
    
    ss->class_omega = new omega_suffstats*[num_periods];
    //malloc(sizeof(omega_suffstats*)*num_periods);
    for (i = 0; i < num_periods; i++)
    {
        ss->class_omega[i]= new omega_suffstats;
        //malloc(sizeof(omega_suffstats));
        ss->class_omega[i]->class_doc_total = new double[num_topics_citation];
        //malloc(sizeof(double)*num_topics);
        ss->class_omega[i]->class_doc = new double*[num_topics_citation];
        //malloc(sizeof(double*)*num_topics);
        for (j = 0; j < num_topics_citation; j++)
        {
            ss->class_omega[i]->class_doc_total[j] = 0;
            //cout<<model->docs_per_period[i]<<endl;
            ss->class_omega[i]->class_doc[j] = new double[model->docs_per_period[i]];
            //malloc(sizeof(double)*model->docs_per_period[i]);
            for (k = 0; k < model->docs_per_period[i]; ++k)
            {
                ss->class_omega[i]->class_doc[j][k] = 0;
            }
        }
    }
    
    return(ss);
}
//
///*
// * various intializations for the sufficient statistics
// *
// */
//
void zero_initialize_ss(lda_suffstats* ss, lda_model* model)
{
    int k, w;
    for (k = 0; k < model->num_topics_word; k++)
    {
        ss->class_total[k] = 0;
        for (w = 0; w < model->num_terms; w++)
        {
            ss->class_word[k][w] = 0;
        }
    }
    
    int t1, t2;
    for (t1 = 0; t1 < model->num_topics_word; ++t1)
    {
        ss->class_topic_total[t1] = 0;
        for (t2 = 0; t2 < model->num_topics_citation; ++t2)
        {
            ss->class_topic[t1][t2] = 0;
        }
    }
    
    int period, t, d;
    for (period = 0; period < model->num_periods; ++period)
    {
        for (t = 0; t < model->num_topics_citation; ++t)
        {
            ss->class_omega[period]->class_doc_total[t] = 0;
            for (d = 0; d < model->docs_per_period[period]; ++d)
            {
                ss->class_omega[period]->class_doc[t][d] = 0;
            }
        }
    }
    
    ss->num_docs = 0;
    ss->alpha_suffstats = 0;
}


void random_initialize_ss(lda_suffstats* ss, lda_model* model)
{
    int num_topics_word = model->num_topics_word;
    int num_topics_citation=model->num_topics_citation;
    int num_terms = model->num_terms;
    int num_periods = model->num_periods;
    int k, n;
    for (k = 0; k < num_topics_word; k++)
    {
        for (n = 0; n < num_terms; n++)
        {
            ss->class_word[k][n] += 1.0/num_terms + myrand();
            ss->class_total[k] += ss->class_word[k][n];
        }
    }
    
    int t1, t2;
    for (t1 = 0; t1 < num_topics_word; ++t1)
    {
        for (t2 = 0; t2 < num_topics_citation; ++t2)
        {
            ss->class_topic[t1][t2] += 1.0/num_topics_citation + myrand();
            ss->class_topic_total[t1] += ss->class_topic[t1][t2];
        }
    }
    
    int period, t, d;
    for (period = 0; period < num_periods; ++period)
    {
        for (t = 0; t < num_topics_citation; ++t)
        {
            
            for (d = 0; d < model->docs_per_period[period]; ++d)
            {
                
                ss->class_omega[period]->class_doc[t][d] += 1.0/model->docs_per_period[period] + myrand();
                ss->class_omega[period]->class_doc_total[t] += ss->class_omega[period]->class_doc[t][d];
            }
        }
    }
}


void corpus_initialize_ss(lda_suffstats* ss, lda_model* model, dataset* c)
{
    int num_topics_word = model->num_topics_word;
    int num_topics_citation=model->num_topics_citation;
    int num_periods = model->num_periods;
    int i, k, d, n;
    int period;
    document* doc;
    
    for (k = 0; k < num_topics_word; k++)
    {
        for (i = 0; i < NUM_INIT; i++)
        {
            d = floor(myrand() * c->num_docs);
            printf("initialized beta with document %d\n", d);
            doc = c->docs[d];
            for (n = 0; n < doc->length; n++)
            {
                ss->class_word[k][doc->words[n]] += doc->counts[n];
            }
        }
        for (n = 0; n < model->num_terms; n++)
        {
            ss->class_word[k][n] += 1.0;
            ss->class_total[k] = ss->class_total[k] + ss->class_word[k][n];
        }
        
        
    }
    
    for (period = 0; period < num_periods; period++)
    {
        for(k = 0; k < num_topics_citation; k++)
        {
            for (i = 0; i < NUM_INIT; i++)
            {
                
                if (period==0) {
                    d=floor(myrand()*c->docs_per_period[period]);
                }
                else
                {
                    d = floor(myrand() * (c->docs_per_period[period] - c->docs_per_period[period-1] ));
                    d += c->docs_per_period[period-1];
                    
                }
                
                
                printf("intialized omega with document %d\n", d);
                doc = c->docs[d];
                for (n = 0; n < doc->cit_length; n++)
                {
                    if(doc->citation[n] <= c->docs_per_period[period])         //in case that the document cite the future
                    {
                        ss->class_omega[period]->class_doc[k][doc->citation[n]] += 1;
                    }
                    else
                        printf("something error");
                }
                
            }
            for (n = 0; n < model->docs_per_period[period]; n++)
            {
                ss->class_omega[period]->class_doc[k][n] += 1.0;
                ss->class_omega[period]->class_doc_total[k] = ss->class_omega[period]->class_doc_total[k] + ss->class_omega[period]->class_doc[k][n];
            }
            
        }
    }
    
    
    //random initialize
    int t1, t2;
    for (t1 = 0; t1 < num_topics_word; ++t1)
    {
        for (t2 = 0; t2 < num_topics_citation; ++t2)
        {
            ss->class_topic[t1][t2] += 1.0/num_topics_citation + myrand();
            ss->class_topic_total[t1] += ss->class_topic[t1][t2];
        }
    }
}
/*
 * allocate new lda model
 *
 */

lda_model* new_lda_model(int num_terms, int num_topics_word,int num_topics_citation,int num_periods,int *doc_per_period)
{   //cout<<num_topics<<endl;
    int i,j,k,t;
    lda_model* model;
   // cout<<num_periods;
    
    model = new lda_model;
    model->num_periods =num_periods;
    model->num_topics_word = num_topics_word;
    model->num_topics_citation=num_topics_citation;
    model->num_terms = num_terms;
    model->alpha = 1.0;
    model->docs_per_period =new int[num_periods];
    model->log_prob_w = new double*[num_topics_word];
    //malloc(sizeof(double*)*num_topics);
    model->log_zeta= new double*[num_topics_word];
    //malloc(sizeof(double*)*num_topics);
    model->log_omega= new top_doc*[num_periods];
   // malloc(sizeof(top_doc*)*num_periods);
    
    for (i = 0; i < num_topics_word; i++)
    {
        model->log_prob_w[i] = new double[num_terms];
        //malloc(sizeof(double)*num_terms);
        model->log_zeta[i]= new double[num_topics_citation];
        //malloc(sizeof(double)*num_topics);
        for (j = 0; j < num_terms; j++)
            model->log_prob_w[i][j] = 0;
        for (k=0; k<num_topics_citation; k++) {
            model->log_zeta[i][k]=0;
        }
    }
    
    for (t=0; t<num_periods; t++) {
        model->log_omega[t] = new top_doc;
        //malloc(sizeof(top_doc));
        model->log_omega[t]->log_omega_t=new double*[num_topics_citation];
        model->docs_per_period[t]=doc_per_period[t];
        //malloc(sizeof(double*)*num_topics);
        
        for (i=0; i<num_topics_citation; i++) {
            model->log_omega[t]->log_omega_t[i]= new double[doc_per_period[t]];
            //malloc(sizeof(double)*doc_per_period[t]);
            
            for (j=0; j<doc_per_period[t]; j++) {
                model->log_omega[t]->log_omega_t[i][j]=0;
            }
        }
        
    }
    
    return(model);
}

//
///*
// * deallocate new lda model
// *
// */
//
void free_lda_model(lda_model* model)
{
    int i,t;
    
    for (i = 0; i < model->num_topics_word; i++)
    {
        free(model->log_prob_w[i]);
        free(model->log_zeta[i]);
    }
    
    for (t=0; t<model->num_periods; t++) {
        for (i=0; i<model->num_topics_citation; i++) {
            free(model->log_omega[t]->log_omega_t[i]);
        }
        free(model->log_omega[t]);
    }
    
    free(model->log_omega);
    free(model->log_zeta);
    free(model->log_prob_w);
    
}



///*
// * save an lda model
// *
// */

void save_lda_model(lda_model* model, char* model_root)
{
    char filename[100];
    FILE* fileptr;
    int i, j,t;
    
    sprintf(filename, "%s.beta", model_root);
    fileptr = fopen(filename, "w");
    for (i = 0; i < model->num_topics_word; i++)
    {
        fprintf(fileptr, "%5.10f",exp( model->log_prob_w[i][0]));
        for (j = 1; j < model->num_terms; j++)
        {
            fprintf(fileptr, " %5.10f", exp(model->log_prob_w[i][j]));
        }
        if (i!=model->num_topics_word-1) {
            fprintf(fileptr, "\n");
        }
        
    }
    fclose(fileptr);
    
    sprintf(filename, "%s.zeta", model_root);
    fileptr = fopen(filename, "w");
    for (i = 0; i < model->num_topics_word; i++)
    {
        fprintf(fileptr, "%5.10f",exp( model->log_zeta[i][0]));
        for (j = 1; j < model->num_topics_citation; j++)
        {
            fprintf(fileptr, " %5.10f", exp(model->log_zeta[i][j]));
        }
        if (i!=model->num_topics_word-1) {
            fprintf(fileptr, "\n");
        }
    }
    fclose(fileptr);
    
    
    for (t=0; t<model->num_periods;t++) {
        sprintf(filename, "%s.omega%d", model_root,t);
        fileptr = fopen(filename, "w");
        for (i = 0; i < model->num_topics_citation; i++)
        {
            fprintf(fileptr, "%5.10f",exp( model->log_omega[t]->log_omega_t[i][0]));
            for (j = 1; j < model->docs_per_period[t]; j++)
            {
                fprintf(fileptr, " %5.10f", exp(model->log_omega[t]->log_omega_t[i][j]));
            }
            if (i!=model->num_topics_citation-1) {
                fprintf(fileptr, "\n");
            }
            
        }
        fclose(fileptr);
    }
    
    
    sprintf(filename, "%s.other", model_root);
    fileptr = fopen(filename, "w");
    fprintf(fileptr, "num_topics_word %d\n", model->num_topics_word);
    fprintf(fileptr, "num_topics_citation %d\n", model->num_topics_citation);
    fprintf(fileptr, "num_terms %d\n", model->num_terms);
    fprintf(fileptr, "alpha %5.10f\n", model->alpha);
    fclose(fileptr);
}

//
////load initial parameters from file root,including alpha,beta
//lda_model* load_lda_model(char* model_root)
//{
//    char filename[100];
//    FILE* fileptr;
//    int i, j, num_terms, num_topics;
//    float x, alpha;
//
//    sprintf(filename, "%s.other", model_root);
//    printf("loading %s\n", filename);
//    fileptr = fopen(filename, "r");
//    fscanf(fileptr, "num_topics %d\n", &num_topics);
//    fscanf(fileptr, "num_terms %d\n", &num_terms);
//    fscanf(fileptr, "alpha %f\n", &alpha);
//    fclose(fileptr);
//
//    lda_model* model = new_lda_model(num_terms, num_topics);
//    model->alpha = alpha;
//
//    sprintf(filename, "%s.beta", model_root);
//    printf("loading %s\n", filename);
//    fileptr = fopen(filename, "r");
//    for (i = 0; i < num_topics; i++)
//    {
//        for (j = 0; j < num_terms; j++)
//        {
//            fscanf(fileptr, "%f", &x);
//            model->log_prob_w[i][j] = x;
//        }
//    }
//    fclose(fileptr);
//    return(model);
//}