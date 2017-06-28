//
//  dataset.h
//  evolda
//
//  Created by huangxin on 15/12/13.
//  Copyright © 2015年 huangxin. All rights reserved.
//

#ifndef dataset_h
#define dataset_h
#include <stdio.h>
#include <string>
#include <vector>
#include <map>
using namespace std;

extern int NPERIODS;
// map of words/terms [string => int]
typedef map<string, int> mapword2id;
// map of words/terms [int => string]
typedef map<int, string> mapid2word;
typedef map<int, int> mapindex2id;
typedef map<int, int> mapid2index;

class document {
public:
    int * words;//to be editted
    string rawstr;
    int total;//here i guess is the length of the whole document.
    int length; //this is the number of the kinds of words
    int index;
    int index_old;
    int * citation;
    int cit_length;
    int period=-1;
    int year;
    int* counts;
    
    
    document() {
        words = NULL;
        counts = NULL;
        rawstr = "";
        total = 0;
        //period= -1;
        index_old = 0;
        citation = NULL;
        cit_length = 0;
        length =0;
        year = 0;
    }
    
    document(int total, int cit_length) {
        this->total = total;
        rawstr = "";
        words = new int[total];
        counts = new int[total];
        
        this->cit_length =cit_length;
        citation = new int[cit_length];
        length =0;
        index_old =0;
        year =0;
        
    }
    
    document(int total, int * words, int cit_length, int* citation) {
        this->total = total;
        rawstr = "";
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = words[i];
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        
        this->cit_length =cit_length;
        citation = new int[cit_length];
        for(int i=0; i<cit_length; i++) {
            this->citation[i]= citation[i];
        }
        
        index_old =0;
        year =0;
    }
    
    document(int total, int * words, string rawstr, int cit_length, int* citation) {
        this->total = total;
        this->rawstr = rawstr;
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = words[i];
            
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        this->cit_length =cit_length;
        citation = new int[cit_length];
        for(int i=0; i<cit_length; i++) {
            this->citation[i]= citation[i];
        }
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        index_old = 0;
        year = 0;
    }
    
    
    document(int total, int * words, string rawstr, int index_old, int cit_length, int* citation) {
        this->total= total;
        this->rawstr = rawstr;
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = words[i];
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        this->index_old = index_old;
        this->cit_length =cit_length;
        citation = new int[cit_length];
        for(int i=0; i<cit_length; i++) {
            this->citation[i]= citation[i];
        }
        
        year =0;
    }
    
    
    document(int total, int * words, string rawstr, int index_old, int cit_length, int* citation, int period){
        this->total = total;
        this->rawstr = rawstr;
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = words[i];
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        this->index_old = index_old;
        this->cit_length=cit_length;
        citation = new int[cit_length];
        for(int j = 0; j < cit_length; j++) {
            this->citation[j] = citation[j];
        }
        this->year = period;
    }
    
    
    
    document(vector<int> & doc, vector<int> & cit) {
        this->total = doc.size();
        rawstr = "";
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = doc[i];
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        
        this->cit_length = cit.size();
        this->citation = new int[cit_length];
        for (int j = 0; j < cit_length; j++) {
            this->citation[j]=cit[j];
        }
        
        index_old =0;
        period =0;
    }
    
    
    document(vector<int> & doc, string rawstr, vector<int> & cit) {
        this->total= doc.size();
        this->rawstr = rawstr;
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = doc[i];
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        
        this->cit_length = cit.size();
        this->citation = new int[cit_length];
        for (int j = 0; j < cit_length; j++) {
            this->citation[j]=cit[j];
        }
        
        index_old =0;
        year =0;
    }
    
    document(vector<int> & doc, string rawstr, vector<int> & cit, int index_old) {
        this->total = doc.size();
        this->rawstr = rawstr;
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = doc[i];
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        
        this->cit_length = cit.size();
        this->citation = new int[cit_length];
        for (int j = 0; j < cit_length; j++) {
            this->citation[j]=cit[j];
        }
        this->index_old = index_old;
        
        period =0;
    }
    
    document(vector<int> & doc, string rawstr, vector<int> & cit, int index_old, int period) {
        this->total = doc.size();
        this->rawstr = rawstr;
        this->words = new int[total];
        for (int i = 0; i < total; i++) {
            this->words[i] = doc[i];
        }
        counts = new int[total];
        for(int n=0; n<total ;n++){
            for(int fuck=0; fuck<=n; fuck++) {
                if(words[n]==words[fuck])
                    counts[words[n]]++ ;
            }
        }
        
        this->cit_length = cit.size();
        this->citation = new int[cit_length];
        for (int j = 0; j < cit_length; j++) {
            this->citation[j]=cit[j];
        }
        this->index_old = index_old;
        this->year = period;
    }
    
    ~document() {
        if (words) {
            delete words;
        }
        if(counts) {
            delete counts;
        }
        if(citation){
            delete citation;
        }
    }
};

class dataset {
public:
    document ** docs; //why 2  "**"
    document ** _docs; // used only for inference
    map<int, int> _id2id; // also used only for inference //for now i am not sure what it means
    int ** reference_matrix;
    
    
    int num_docs; // number of documents
    int num_term; // number of words
    int num_period;
    int* docs_per_period;//number of period
    
    dataset() {
        docs = NULL;
        _docs = NULL;
        num_docs = 0;
        num_term = 0;
        num_period = NPERIODS;
        this->docs_per_period = new int[num_period];
    }
    
    dataset(int num_docs) {
        this->num_docs = num_docs ;
        this->num_term = 0;
        docs = new document*[num_docs];
        _docs = NULL;
    }
    
    dataset(int num_docs, int num_period) {
        this->num_docs = num_docs ;
        this->num_term = 0;
        docs = new document*[num_docs];
        _docs = NULL;
        this->num_period = num_period;
        this->docs_per_period = new int[num_period];
    }
    
    
    ~dataset() {
        if (docs) {
            for (int i = 0; i < num_docs; i++) {
                delete docs[i];
            }
        }
        delete docs;
        
        if (_docs) {
            for (int i = 0; i < num_docs; i++) {
                delete _docs[i];
            }
        }
        delete _docs;
        
        
    }
    
    void deallocate() {
        if (docs) {
            for (int i = 0; i < num_docs; i++) {
                delete docs[i];
            }
        }
        delete docs;
        docs = NULL;
        
        if (_docs) {
            for (int i = 0; i < num_docs; i++) {
                delete _docs[i];
            }
        }
        delete _docs;
        _docs = NULL;
        
        delete docs_per_period;
        docs_per_period = NULL;
    }
    
    
    void add_doc(document * doc, int idx) {
        if (0 <= idx && idx < num_docs) {
            docs[idx] = doc;
        }
    }
    
    void _add_doc(document * doc, int idx) {
        if (0 <= idx && idx < num_docs) {
            _docs[idx] = doc;
        }
    }
    
    static int write_wordmap(string wordmapfile, mapword2id * pword2id);
    static int read_wordmap(string wordmapfile, mapword2id * pword2id);
    static int read_wordmap(string wordmapfile, mapid2word * pid2word);
    
    static int write_citmap(string indexmapfile, mapindex2id * pindex2id);
    static int read_citmap(string indexmapfile, mapindex2id * pcit2id);
    static int read2_citmap(string indexmapfile, mapid2index * pid2cit);
    
    int read_trndata(string dfile, string wordmapfile, string cfile, string citmapfile, string tfile);
    //int read_newdata(string dfile, string wordmapfile, string cfile, string citmapfile, string tfile);
    //int read_newdata_withrawstrs(string dfile, string wordmapfile);
};

typedef struct
{
    double** class_doc;
    double* class_doc_total;
}omega_suffstats;

typedef struct{
    int period;
    double** log_omega_t;
} top_doc;

typedef struct
{
    double alpha;
    double** log_prob_w;
    int num_topics_word;
    int num_topics_citation;
    int num_terms;
    int num_periods;
    int *docs_per_period;
    
    ///new variables are added here
    
    double **log_zeta;// a matrice K*K with k=num_topic
    top_doc **log_omega;
    
} lda_model;


typedef struct
{
    double** class_word;
    double* class_total;
    double alpha_suffstats;
    int num_docs;
    
    double** class_topic;            //newly added
    double* class_topic_total;       //newly added
    
    omega_suffstats **class_omega;   //newly added
    /// new variable are added here
    // double** class_citation;
    // double** class_topic;
    
} lda_suffstats;



#endif /* dataset_h */
