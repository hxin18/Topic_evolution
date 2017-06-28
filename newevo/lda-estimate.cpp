//
//  lda-estimate.cpp
//  evolda
//
//  Created by huangxin on 15/12/13.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#include <stdio.h>

#include <stdio.h>
#include "dataset.h"
#include <iostream>
#include "sorting.h"
#include "seperate.h"
#include "lda-estimate.h"
#include "lda-inference.h"
#include <fstream>


float EM_CONVERGED=0.000001;
int EM_MAX_ITER=50;
int ESTIMATE_ALPHA=1;
double INITIAL_ALPHA;
int NTOPICS_WORD;
int NTOPICS_CITATION;
int NPERIODS;

void save_gamma(char* filename, double** gamma, int num_docs, int num_topics)
{
    FILE* fileptr;
    int d, k;
    fileptr = fopen(filename, "w");
    
    for (d = 0; d < num_docs; d++)
    {
        float aa= gamma[d][0];
        for (k = 1; k < num_topics; k++)
        {
           aa+= gamma[d][k];
        }
     
        fprintf(fileptr, "%5.10f", gamma[d][0]/aa);
        for (k = 1; k < num_topics; k++)
        {
            fprintf(fileptr, " %5.10f", gamma[d][k]/aa);
        }
        fprintf(fileptr, "\n");
    }
    fclose(fileptr);
}


void run_em(char *start, char *directory, dataset *dataset)
{
    int d,n,l;
    int **topic_assign;
    int **topic_assign_citation;
    topic_assign = new int*[dataset->num_docs];
    topic_assign_citation=new int*[dataset->num_docs];
    for(d = 0; d < dataset->num_docs; d++)
    {
        topic_assign[dataset->docs[d]->index] = new int[dataset->docs[d]->length];

        //
        topic_assign_citation[dataset->docs[d]->index]=new int[dataset->docs[d]->cit_length];
//        for (int j=0; j<dataset->docs[d]->cit_length; j++) {
//            topic_assign_citation[d][j]=0;
//        }
    }
    
    
    
    double **var_gamma,**phi,**lamda,**sigma;
   // cout<<"sb";
    
    var_gamma = new double*[dataset->num_docs];
   // malloc(sizeof((double*)*(dataset->num_docs));
    for (d = 0; d < dataset->num_docs; d++) {
        var_gamma[d] = new double[NTOPICS_WORD];
        //malloc(sizeof(double) * NTOPICS);
    }
    
    int max_length = max_corpus_length(dataset);
    phi = new double*[max_length];
    //malloc(sizeof(double*)*max_length);
    
    for (n = 0; n < max_length; n++) {
        phi[n] = new double[NTOPICS_WORD];
        //malloc(sizeof(double) * NTOPICS);
    }
    
    int max_citlength=max_corpus_citlength(dataset);
    lamda=  new double*[max_citlength];
   // malloc(sizeof(double*)*max_citlength);
    
    sigma= new double*[max_citlength];
    //malloc(sizeof(double*)*max_citlength);
    
    for (l=0; l<max_citlength; l++) {
        lamda[l]= new double[NTOPICS_WORD];
        //malloc(sizeof(double)*NTOPICS);
        sigma[l]= new double[NTOPICS_CITATION];
        //malloc(sizeof(double)*NTOPICS);
    }
    
    lda_model *model ;
    lda_suffstats* ss ;
    
    // initialize model
    
    char filename[200];
    
  /*  if (strcmp(start, "seeded")==0)
    {
        model = new_lda_model(dataset->num_term, NTOPICS,NPERIODS,dataset->docs_per_period);
        ss = new_lda_suffstats(model);
        corpus_initialize_ss(ss, model, dataset);
        lda_mle(model, ss, 0);
        model->alpha = INITIAL_ALPHA;
    }
    else if (strcmp(start, "random")==0)
    {*/
        model = new_lda_model(dataset->num_term, NTOPICS_WORD,NTOPICS_CITATION,dataset->num_period,dataset->docs_per_period);
        //cout<<model->num_periods<<endl;
    
        ss = new_lda_suffstats(model);
        random_initialize_ss(ss, model);
        lda_mle(model, ss, 0);
        model->alpha = INITIAL_ALPHA;
        //sprintf(filename, "%s/original.dat", directory);
   // }
    //    else
    //    {
    //        model = load_lda_model(start);
    //        ss = new_lda_suffstats(model);
    //    }
    
//        sprintf(filename,"%s/000",directory);
//        save_lda_model(model, filename);
    
    
    int i = 0;
    double likelihood, likelihood_old = 1, converged = 1;
    sprintf(filename, "%s/likelihood.dat", directory);
    FILE* likelihood_file = fopen(filename, "w");
    
    while (((converged < 0) || (converged > EM_CONVERGED) || (i <= 2)) && (i <= EM_MAX_ITER))
    {
        i++; printf("**** em iteration %d ****\n", i);
        likelihood = 0;
        zero_initialize_ss(ss, model);
        // e-step,update phi and gamma
        //因为只用了一个phi，所以M步的更新必须通过sufficient来进行
        for (d=0; d<dataset->num_docs; d++) {
            if ((d % 1000) == 0) printf("document %d\n",d);
            likelihood += doc_e_step((dataset->docs[d]),
                                     var_gamma[d],
                                     phi,
                                     lamda,
                                     sigma,
                                     model,
                                     ss,
                                     topic_assign,
                                     topic_assign_citation,
                                     false);
        }
//        
//        sprintf(filename,"%s/original.gamma",directory);
//        save_gamma(filename, var_gamma, dataset->num_docs, model->num_topics);
//
        // m-step,update alpha and beta
        
        lda_mle(model, ss, ESTIMATE_ALPHA);
        
        // check for convergence
        
        converged = (likelihood_old - likelihood) / (likelihood_old);
        
        likelihood_old = likelihood;
        
        // output model and likelihood
        cout<<likelihood<<' '<<converged<<endl;
        
        
        fprintf(likelihood_file, "%10.10f\t%5.5e\n", likelihood, converged);
        fflush(likelihood_file);
        if ((i % LAG) == 0)
        {
            sprintf(filename,"%s/%03d",directory, i);
            save_lda_model(model, filename);
            sprintf(filename,"%s/%03d.gamma",directory, i);
            save_gamma(filename, var_gamma, dataset->num_docs, model->num_topics_word);
        }
        if (converged < 0)
        {
            sprintf(filename,"%s/%s",directory, "negative");
            save_lda_model(model, filename);
            sprintf(filename,"%s/%s.gamma",directory, "negative");
            save_gamma(filename, var_gamma, dataset->num_docs, model->num_topics_word);
            break;
//            VAR_MAX_ITER = VAR_MAX_ITER * 2;
        }
    }
    int tmp;
    zero_initialize_ss(ss, model);
    for (d=0; d<dataset->num_docs; d++) {
        if ((d % 1000) == 0) printf("document %d\n",d);
        tmp=doc_e_step((dataset->docs[d]),
                   var_gamma[d],
                   phi,
                   lamda,
                   sigma,
                   model,
                   ss,
                   topic_assign,
                   topic_assign_citation,
                   true);   ///flag is converged
    }
    
    
    // output the final model
    
    sprintf(filename,"%s/final",directory);
    save_lda_model(model, filename);
    
    write_assignment(directory, dataset, topic_assign, topic_assign_citation,model);
    
    sprintf(filename,"%s/final.gamma",directory);
    save_gamma(filename, var_gamma, dataset->num_docs, model->num_topics_word);
    
    // output the word assignments (for visualization)
    
    //    sprintf(filename, "%s/word-assignments.dat", directory);
    //    FILE* w_asgn_file = fopen(filename, "w");
    //    for (d = 0; d < corpus->num_docs; d++)
    //    {
    //        if ((d % 100) == 0) printf("final e step document %d\n",d);
    //        likelihood += lda_inference(&(corpus->docs[d]), model, var_gamma[d], phi);
    //        write_word_assignment(w_asgn_file, &(corpus->docs[d]), phi, model);
    //    }
    //    fclose(w_asgn_file);
    //    fclose(likelihood_file);
    
    
    
    
    
}

double doc_e_step(document *doc, double *gamma,
                  double **phi, double **lamda, double **sigma,
                  lda_model *model,lda_suffstats *ss,int **topic_assign,
                  int **topic_assign_citation,bool flag)
{
    double likelihood;
    likelihood=lda_inference(doc,gamma,phi,lamda,sigma,model,topic_assign,topic_assign_citation,flag);
    int k=0,n=0,i=0,j=0,l=0,p=0,q=0;
    // update sufficient statistics
    
    double gamma_sum = 0;
    for (k = 0; k < model->num_topics_word; k++)
    {
        gamma_sum += gamma[k];
        ss->alpha_suffstats += digamma(gamma[k]);
    }
    ss->alpha_suffstats -= model->num_topics_word * digamma(gamma_sum);
    
    for (n = 0; n < doc->length; n++)
    {
        for (k = 0; k < model->num_topics_word; k++)
        {
            ss->class_word[k][doc->words[n]] += doc->counts[n]*phi[n][k];
            ss->class_total[k] += doc->counts[n]*phi[n][k];
        }
    }
    
    for (i=0; i<model->num_topics_word; i++) {
        for (j=0; j<model->num_topics_citation; j++) {
            for (l=0; l<doc->cit_length; l++) {
                ss->class_topic[i][j]+=sigma[l][j]*lamda[l][i];
                ss->class_topic_total[i]+=sigma[l][j]*lamda[l][i];
            }
        }
    }
//    tmp2=ss->class_omega[1]->class_doc;
    for (p=0; p<doc->cit_length; p++) {
        for (q=0; q<model->num_topics_citation; q++) {
            ss->class_omega[doc->period]->class_doc[q][doc->citation[p]]+=sigma[p][q];//sigma[p][q];
            ss->class_omega[doc->period]->class_doc_total[q]+=sigma[p][q];
        }
    }
    
    ss->num_docs = ss->num_docs + 1;
    
    return(likelihood);
}


void write_assignment(char* directory, dataset *dataset , int** topic_assign,
                           int **topic_assign_citation,lda_model* model)
{
    int n;
    int d;
    char filename[200];
    ofstream fout1;
    ofstream fout2;
    sprintf(filename, "%s/wordassignment.dat",directory);
    fout1.open(filename);
    sprintf(filename, "%s/citationassignment.dat",directory);
    fout2.open(filename);
    
        ///fprintf(f, "%03d", doc->length);
    for (d = 0; d < dataset->num_docs; d++)
    {
//        fprintf(f, "%03d", dataset->docs[d]->length);
//        for (n = 0; n < dataset->docs[d]->length; n++)
//        {
//            fprintf(f, " %04d:%02d",
//                    dataset->docs[d]->words[n], topic_assign[d][n]);
//        }
//        fout1<<dataset->docs[d]->length;
        for (n=0; n<dataset->docs[d]->length; n++) {
            fout1<<topic_assign[dataset->docs[d]->index][n]<<' '<<dataset->docs[d]->counts[n]<<' ';
        }
//        if (d!=dataset->num_docs-1) {
//            fout1<<'\n';
//        }
    }
    
    for (d = 0; d < dataset->num_docs; d++)
    {
        //        fprintf(f, "%03d", dataset->docs[d]->length);
        //        for (n = 0; n < dataset->docs[d]->length; n++)
        //        {
        //            fprintf(f, " %04d:%02d",
        //                    dataset->docs[d]->words[n], topic_assign[d][n]);
        //        }
//        fout2<<dataset->docs[d]->cit_length;
        for (n=0; n<dataset->docs[d]->cit_length; n++) {
            fout2<<topic_assign_citation[dataset->docs[d]->index][n]<<' ';
        }
//        if (d!=dataset->num_docs-1) {
//            fout2<<'\n';
//        }
    }
    fout1.close();
    fout2.close();
}



/*
 * read settings.
 *
 */

void read_settings(char* filename)
{
    FILE* fileptr;
    char alpha_action[100];
    fileptr = fopen(filename, "r");
//    fscanf(fileptr, "var max iter %d\n", &VAR_MAX_ITER);
//f    fscanf(fileptr, "var convergence %f\n", &VAR_CONVERGED);
    fscanf(fileptr, "em max iter %d\n", &EM_MAX_ITER);
    fscanf(fileptr, "em convergence %f\n", &EM_CONVERGED);
    fscanf(fileptr, "alpha %s", alpha_action);
    if (strcmp(alpha_action, "fixed")==0)
    {
        ESTIMATE_ALPHA = 0;
    }
    else
    {
        ESTIMATE_ALPHA = 1;
    }
    fclose(fileptr);
}
int main(int argc, const char * argv[]) {
    //cout<<"sb";
    INITIAL_ALPHA = 0.01;
    NTOPICS_CITATION = 100;
    NTOPICS_WORD=100;
    NPERIODS=1;
    map<int,int> oldtonew;
    dataset* ptrndata = new dataset();
    long t1;
    (void) time(&t1);
    seedMT(t1);
   // INITIAL_ALPHA = 0.01;
    //char dir='/Users/huangxin/Desktop/evolda/';
    ofstream fout;
    ptrndata->read_trndata("/Volumes/TOSHIBA EXT/citeseer_data/abs.txt", "/Users/huangxin/Documents/毕设/data/annwordmapfile", "/Volumes/TOSHIBA EXT/citeseer_data/cit.txt", "./citmapfile","/Users/huangxin/Documents/毕设/data/citeseer/year.txt");
    
    cout<<"Read Complete!"<<endl;

    
   // time_per_period(ptrndata,1990,1);
    

    cout<<"Num_Docs:"<<ptrndata->num_docs<<'\n';
   // cout<<"Num_Terms:"<<ptrndata->num_term<<'\n';
    cout<<"Num_Periods:"<<ptrndata->num_period<<'\n';
   /* cout<<"Docs_Period_List"<<'\n';
    for (int i=0; i<ptrndata->num_period; i++) {
        cout<<i<<":"<<ptrndata->docs_per_period[i]<<' ';
    }
    cout<<'\n';
    for (int i=0; i<ptrndata->num_docs; i++) {
        if (ptrndata->docs[i]->period!=0) {
            cout<<"period error"<<endl;
        }
        for (int j=0; j<ptrndata->docs[i]->length; j++) {
            if (ptrndata->docs[i]->counts[j]<=0) {
                cout<<"counts error"<<endl;
            }
        }
    }

*/
    
    // read_settings(argv[4]);
     //make_directory(argv[7]);
   // char* a="ramdom";
    char * b ="/Users/huangxin/Desktop/experiment/100-100";
   // cout<<"start!"<<endl;
  //  run_em(a, b, ptrndata);
   // cout<<"end"<<endl;
    return 0;
}

