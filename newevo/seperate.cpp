//
//  seperate.cpp
//  evolda
//
//  Created by huangxin on 15/12/14.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#include <stdio.h>
#include "seperate.h"
#include <iostream>

void time_per_period(dataset *dataset, int first_year, int gap){
    for(int i=0;i<dataset->num_period;i++) dataset->docs_per_period[i]=0;
    for(int i=0; i<dataset->num_docs; i++) {
        for(int j=0;j<dataset->num_period;j++){
            if(dataset->docs[i]->year<=first_year+j*gap) {
                dataset->docs_per_period[j]++;
                //std::cout<<i<<":"<<dataset->docs[i]->period<<" ";
                if(dataset->docs[i]->period== -1) {
                    dataset->docs[i]->period= j ;
                    //std::cout<<" "<<dataset->docs[i]->period<<endl;
                }
            }
            // cout<<dataset->docs[i]->year<<"::"<<first_year+(dataset->num_period-1)*gap<<endl;
            if(dataset->docs[i]->year>=first_year+(dataset->num_period-1)*gap){
                //cout<<dataset->docs[i]->year<<"hello";
                dataset->docs[i]->period= dataset->num_period-1;
                //cout<<dataset->docs[i]->period<<endl;
                dataset->docs_per_period[dataset->num_period-1]=dataset->num_docs;
            }
        }
    }
    // dataset->docs_per_period[dataset->num_period-1] = dataset->num_docs;
    
}