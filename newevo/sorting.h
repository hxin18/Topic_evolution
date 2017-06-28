//
//  sorting.h
//  evolda
//
//  Created by huangxin on 15/12/14.
//  Copyright © 2015年 huangxin. All rights reserved.
//
#include"dataset.h"
#ifndef sorting_h
#define sorting_h

map<int,int>  getnewid(dataset* dataset,map<int,int> oldtonew);
void changecitation(dataset* dataset, map<int,int> oldtonew);
int partition(document** unsorted, int low, int high) ;
void quick_sort(document** unsorted,int low, int high);
#endif /* sorting_h */
