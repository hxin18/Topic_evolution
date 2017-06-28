//
//  lda-data.cpp
//  evolda
//
//  Created by huangxin on 15/12/13.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#include <stdio.h>
//
//  lda-data.c
//  TopicEvo
//
//  Created by River on 12/8/15.
//  Copyright © 2015 River. All rights reserved.
//

#include "lda-data.h"


int max_corpus_length(dataset* c)
{
    int n, max = 0;
    for (n = 0; n < c->num_docs; n++)
        if (c->docs[n]->length > max) max = c->docs[n]->length;
    return(max);
}

int max_corpus_citlength(dataset *c)
{
    int n,max=0;
    for (n=0; n<c->num_docs; n++) {
        if (c->docs[n]->cit_length>max) {
            max=c->docs[n]->cit_length;
        }
    }
    
    return max;
}