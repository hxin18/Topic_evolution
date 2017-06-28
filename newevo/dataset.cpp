//
//  dataset.cpp
//  evolda
//
//  Created by huangxin on 15/12/13.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#include "dataset.h"
#include <stdio.h>
#include <stdlib.h>
#include "constant.h"
#include "strtokenizer.h"
#include <iostream>


using namespace std;

int dataset::write_wordmap(string wordmapfile, mapword2id * pword2id) {
    //cout<<wordmapfile.c_str()<<endl;
    FILE * fout = fopen(wordmapfile.c_str(), "w");
    if (!fout) {
        printf("Cannot open file %s to write!\n", wordmapfile.c_str());
        return 1;
    }
    
    mapword2id::iterator it;
    fprintf(fout, "%lu\n", pword2id->size());
    for (it = pword2id->begin(); it != pword2id->end(); it++) {
        fprintf(fout, "%s %d\n", (it->first).c_str(), it->second);
    }
    
    fclose(fout);
    
    return 0;
}

int dataset::write_citmap(string indexmapfile, mapindex2id * pindex2id){
    FILE * fout = fopen(indexmapfile.c_str(),"w");
    if (!fout) {
        printf("Cannot open file %s to write!\n", indexmapfile.c_str());
        return 1;
    }
    mapindex2id::iterator it;
    fprintf(fout, "%lu\n", pindex2id->size());
    for (it = pindex2id->begin(); it != pindex2id->end(); it++) {
        fprintf(fout, "%d %d\n", it->first, it->second);
    }
    
    fclose(fout);
    
    return 0;
    
}

int dataset::read_wordmap(string wordmapfile, mapword2id * pword2id) {
    pword2id->clear();
    
    FILE * fin = fopen(wordmapfile.c_str(), "r");
    if (!fin) {
        printf("Cannot open file %s to read!\n", wordmapfile.c_str());
        return 1;
    }
    
    char buff[BUFF_SIZE_SHORT];
    string line;
    
    fgets(buff, BUFF_SIZE_SHORT - 1, fin);
    int nwords = atoi(buff);
    
    for (int i = 0; i < nwords; i++) {
        fgets(buff, BUFF_SIZE_SHORT - 1, fin);
        line = buff;
        
        strtokenizer strtok(line, " \t\r\n");
        if (strtok.count_tokens() != 2) {
            continue;
        }
        
        pword2id->insert(pair<string, int>(strtok.token(0), atoi(strtok.token(1).c_str())));
    }
    
    fclose(fin);
    
    return 0;
}

int dataset::read_citmap(string indexmapfile, mapindex2id * pcit2id){
    pcit2id->clear();
    FILE * fin = fopen(indexmapfile.c_str(),"r");
    if (!fin) {
        printf("Cannot open file %s to read!\n", indexmapfile.c_str());
        return 1;
    }
    
    char buff[BUFF_SIZE_SHORT];
    string line;
    
    fgets(buff, BUFF_SIZE_SHORT - 1, fin);
    int ncit = atoi(buff);
    
    for (int i = 0; i < ncit; i++) {
        fgets(buff, BUFF_SIZE_SHORT - 1, fin);
        line = buff;
        
        strtokenizer strtok(line, " \t\r\n");
        if (strtok.count_tokens() != 2) {
            continue;
        }
        
        pcit2id->insert(pair<int, int>(atoi(strtok.token(0).c_str()), atoi(strtok.token(1).c_str())));
    }
    
    fclose(fin);
    
    return 0;
    
    
}

int dataset::read_wordmap(string wordmapfile, mapid2word * pid2word) {
    pid2word->clear();
    
    FILE * fin = fopen(wordmapfile.c_str(), "r");
    if (!fin) {
        printf("Cannot open file %s to read!\n", wordmapfile.c_str());
        return 1;
    }
    
    char buff[BUFF_SIZE_SHORT];
    string line;
    
    fgets(buff, BUFF_SIZE_SHORT - 1, fin);
    int nwords = atoi(buff);
    
    for (int i = 0; i < nwords; i++) {
        fgets(buff, BUFF_SIZE_SHORT - 1, fin);
        line = buff;
        
        strtokenizer strtok(line, " \t\r\n");
        if (strtok.count_tokens() != 2) {
            continue;
        }
        
        pid2word->insert(pair<int, string>(atoi(strtok.token(1).c_str()), strtok.token(0)));
    }
    
    fclose(fin);
    
    return 0;
}

int dataset::read2_citmap(string indexmapfile, mapid2index * pid2cit){
    pid2cit->clear();
    FILE * fin = fopen(indexmapfile.c_str(),"r");
    if (!fin) {
        printf("Cannot open file %s to read!\n", indexmapfile.c_str());
        return 1;
    }
    
    char buff[BUFF_SIZE_SHORT];
    string line;
    
    fgets(buff, BUFF_SIZE_SHORT - 1, fin);
    int ncit = atoi(buff);
    
    for (int i = 0; i < ncit; i++) {
        fgets(buff, BUFF_SIZE_SHORT - 1, fin);
        line = buff;
        
        strtokenizer strtok(line, " \t\r\n");
        if (strtok.count_tokens() != 2) {
            continue;
        }
        
        pid2cit->insert(pair<int, int>(atoi(strtok.token(1).c_str()), atoi(strtok.token(0).c_str())));
    }
    
    fclose(fin);
    
    return 0;
    
    
}



int dataset::read_trndata(string dfile, string wordmapfile, string cfile, string citmapfile, string tfile ) {
    mapword2id word2id;
    mapindex2id index2id;
    
    //FILE * fin = fopen(dfile.c_str(), "r");
   // if (!fin) {
     //   printf("Cannot open file %s to read!\n", dfile.c_str());
       // return 1;
    //}
    
    FILE * cfin = fopen(cfile.c_str(),"r");
    if(!cfin)
    {
        printf("Cannot open file %s to read!\n",cfile.c_str());
        return 1;
    }
  //  FILE * tfin = fopen(tfile.c_str(), "r");
  //  if(!tfin)
    //{
     //   printf("Cannot open file %s to read!\n",tfile.c_str());
      //  return 1;
    //}
    
    
    mapword2id::iterator it;
    mapindex2id::iterator it_c;
    
    char buff[BUFF_SIZE_LONG];
    char buff_c[BUFF_SIZE_LONG];
    char buff_t[BUFF_SIZE_LONG];
    string line;
    string line_c;
    string line_t;
    
    // get the number of documents
   // fgets(buff, BUFF_SIZE_LONG - 1, fin);
    num_docs = 716800;
    
    if (num_docs <= 0) {
        printf("No document available!\n");
        return 1;
    }
    
    // allocate memory for corpus
    if (docs) {
        deallocate();
    } else {
        docs = new document*[num_docs];
    }
    
    // set number of words to zero
    num_term = 0;
    
    for (int i = 0; i < num_docs; i++) {
        if (i%1000==0) {
            cout<<i<<endl;
        }
      //  fgets(buff, BUFF_SIZE_LONG - 1, fin);
        fgets(buff_c,BUFF_SIZE_LONG - 1, cfin);
        //fgets(buff_t,BUFF_SIZE_LONG - 1, tfin);
       // line = buff;
        line_c = buff_c;
       // line_t = buff_t;
       // strtokenizer strtok(line, " \t\r\n");
        strtokenizer strtok_c(line_c," \t\r\n");
       // strtokenizer strtok_t(line_t," \t\r\n");
     //   int total = strtok.count_tokens();
        int length_c = strtok_c.count_tokens()-1;
        //cout<<length_c<<endl;
        
      //  if (total <= 0) {
          //  printf("Invalid (empty) document!\n");
          //  deallocate();
           // num_docs = num_term = 0;
           // return 1;
      //  }
        //cout<<total<<endl;
        // allocate new document
        document * pdoc = new document(total,length_c);
        //cout<<"haha"<<endl;
        map<string,int> repeatword;
        map<string,int>::iterator it_in;
        
        int pos=0;
        int store;
   //     for (int j = 0; j < total; j++) {
        //    it = word2id.find(strtok.token(j));
         //   it_in = repeatword.find(strtok.token(j));
          //  if (it == word2id.end()) {
                // word not found, i.e., new word
                // pdoc->words[j] = word2id.size();
     //           word2id.insert(pair<string, int>(strtok.token(j), word2id.size()));
            //    if (it_in == repeatword.end()){
                    //word not found new word in document
                    // cout<<strtok.token(j)<<"-"<<repeatword.size()<<endl;
          //          pdoc->counts[repeatword.size()]=1;
            //        pdoc->words[repeatword.size()]= word2id.size()-1;
                    // pos++;
                    // pdoc->words[repeatword.size()]=it->second;
       //             repeatword.insert(pair<string, int>(strtok.token(j), repeatword.size()));
                    //pdoc->counts[repeatword.size()]=1;
         //       }
        //        else{ pdoc->counts[it_in->second]++;
    //            }
      //      }
        //    else {
          //      if (it_in == repeatword.end()){
                    //word not found new word in document
                    //cout<<repeatword.size()<<"-"<<it->second<<endl;
       //             pdoc->counts[repeatword.size()]=1;
         //           pdoc->words[repeatword.size()]= it->second;
                    // pdoc->words[repeatword.size()]=it->second;
        //            repeatword.insert(pair<string, int>(strtok.token(j), repeatword.size()));
                    //pdoc->counts[repeatword.size()]=1;
       //         }
         //       else{ pdoc->counts[it_in->second]++;
         //       }
         //   }
            
            
        //}
        pdoc->length = repeatword.size();
        
        
        //cout<<pdoc->length<<endl;
        pdoc->index_old = atoi(strtok_c.token(0).c_str());
        
        //new add
        pdoc->index=pdoc->index_old;
        
        
        //cout<<"---"<<length_c<<endl;
        for(int j =1; j < length_c+1; j++) {
            pdoc->citation[j-1] = atoi(strtok_c.token(j).c_str());
        }
        
     //   pdoc->year =atoi(strtok_t.token(0).c_str());
        
        /* for(int n=0; n<=pos ;n++){
         //for(int fuck=0; fuck<=n; fuck++) {
         if(pdoc->words[n]==pdoc->words[fuck])
         pdoc->counts[fuck]++ ;
         }
         }*/
        // add new doc to the corpus
        add_doc(pdoc, i);
        //if(i==0){;for(int fk=0; fk<pdoc->length;fk++){cout<<pdoc->words[fk]<<endl;}}
    }
    
    fclose(fin);
    fclose(cfin);
   // fclose(tfin);
    
    // write word map to file
    if (write_wordmap(wordmapfile, &word2id)) {
        return 1;
    }
    
    //two function are needed to add
    
    // update number of words
    num_term = word2id.size();
    
    return 0;
}



