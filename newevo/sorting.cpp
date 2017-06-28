//
//  sorting.cpp
//  evolda
//
//  Created by huangxin on 15/12/14.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#include <stdio.h>
#include "sorting.h"
#include <algorithm>
#include <iostream>
#include <map>


map<int,int> getnewid(dataset* dataset, map<int,int> oldtonew){
    document** docs;
    //cout<<dataset->num_docs;
    docs = new document*[dataset->num_docs];
    for(int i=0;i<dataset->num_docs;i++){
        docs[i]=dataset->docs[i];
        //cout<<docs[i]->index_old<<endl;
    }
        quick_sort(docs,0,dataset->num_docs-1);
    for(int i=0; i<dataset->num_docs; i++) {
        int signal=docs[i]->index_old;
        dataset->docs[signal]->index=i;
        oldtonew.insert(pair<int, int>(dataset->docs[signal]->index_old,dataset->docs[signal]->index));
      // cout<<dataset->docs[signal-1]->index_old<<" "<<dataset->docs[signal-1]->index<<endl;
        //cout<<oldtonew.find(2)->first;
    }
    return oldtonew;
    
}
void changecitation(dataset* dataset, map<int,int> oldtonew){
   // cout<<oldtonew.find(3)->second<<endl;
    for(int i=0;i<dataset->num_docs;i++){
        for(int j=0;j<dataset->docs[i]->cit_length;j++){
            map<int, int>::iterator it;
            
            it = oldtonew.find(dataset->docs[i]->citation[j]);
            //cout<<dataset->docs[i]->citation[j]<<" ";
            dataset->docs[i]->citation[j] = it->second;
            //cout<<i<<"-"<<j<<" "<<dataset->docs[i]->cit_length<<" ";
           //cout<<":"<<it->second<<endl;
            //cout<<dataset->docs[i]->citation[j]<<" ";
        }
        cout<<endl;
    }
}


int partition(document** unsorted, int low, int high) {
    document* pivot = unsorted[low];
    while (low < high)
    {
        while (low < high && unsorted[high]->year > pivot->year) high--;
        unsorted[low] = unsorted[high];
        while (low < high && unsorted[low]->year<= pivot->year) low++;
        unsorted[high] = unsorted[low];
    }
    //cout<<unsorted[low]->period<<" ";
    unsorted[low] = pivot;
     //cout<<unsorted[low]->period<<endl;
    
    return low;
}

void quick_sort(document** unsorted,int low, int high)
{
    int loc = 0;
    if ( low  < high)
    {
        loc = partition(unsorted, low, high);
        quick_sort(unsorted, low, loc - 1);
        quick_sort(unsorted, loc + 1, high);
    }
}