#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/libstemmer.h"
#include <math.h>

int wc = 0; //word count (중복포함)
int * wcp = &wc; //word count pointer
int wc2 = 0; //word count (중복 미포함)
int * wc2p = &wc2; 
int lc = 0; //line count
int * lcp = &lc; //line count pointer

int nwc = 0; //word count (중복포함)
int * nwcp = &nwc; //word count pointer
int nwc2 = 0; //word count (중복 미포함)
int * nwc2p = &nwc2; 
int nlc = 0; //line count
int * nlcp = &nlc; //line count pointer

GHashTable * nsave;
void print_counter (gpointer key, gpointer value, gpointer userdata) 
{
	char * t = key ;
	int * d = value ;

	printf("(%s, %d, %.3f)\n", t, *d, (*d)*100.0/(*lcp)) ;
}

void save_counter(gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
	int * d = value ;
    FILE * fp = fopen("../data/model_logproba.csv", "aw");
    int * check = g_hash_table_lookup(nsave, t) ;
    
    if (check == NULL){
        fprintf(fp, "%s, %f, 0\n", t, log10f((*d)*100.0/(*lcp)));
    }
    else{
        fprintf(fp, "%s, %f, %f\n", t, log10f((*d)*100.0/(*lcp)), log10f((*check)*100.0/(*nlcp)));
        g_hash_table_remove(nsave, t);
    }
    fclose(fp);
}
void save_counter2(gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
	int * d = value ;
    FILE * fp = fopen("../data/model_logproba.csv", "aw");
    fprintf(fp, "%s, 0, %f\n", t, log10f((*d)*100.0/(*nlcp)));
    fclose(fp);
}

int main () 
{
    GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ;
    GHashTable * ncounter = g_hash_table_new(g_str_hash, g_str_equal) ;
    nsave = ncounter;

	FILE * f = fopen("../data/train.negative.csv", "r") ;
    //FILE * f = fopen("test1.csv", "r") ;
	char * line = 0x0 ;
    char * stemmer = "./stemmer \"";
	size_t r ; 
	size_t n = 0 ;

    GHashTable * linecheck = g_hash_table_new(g_str_hash, g_str_equal) ;
	while (getline(&line, &n, f) >= 0) {
        *lcp=*lcp+1;
		char * t ;
		char * _line = line ;

		for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {   
            int j=0;
            char line[10240];
            for(int i=0; i<strlen(t); i++){
                if(t[i]<65||t[i]>122) continue;
                if(isupper(t[i]))
                    t[j++]=t[i]+32;
                else
                    t[j++]=t[i];
            }
            t[j]='\0';
            struct sb_stemmer * stemmer ;
            stemmer = sb_stemmer_new("english", 0x0) ;
            const char * s;
			int * d ;
            int * d2 = malloc(sizeof(int)) ;
            s = sb_stemmer_stem(stemmer, t, strlen(t)) ;
            d2 = g_hash_table_lookup(linecheck, strdup(s)) ;
            if(d2==NULL){
                d = g_hash_table_lookup(counter, strdup(s)) ;
                if (d == NULL) {
                    *wc2p=*wc2p+1;
                    d = malloc(sizeof(int)) ;
                    *d = 1 ;
                    g_hash_table_insert(counter, strdup(s), d);
                    g_hash_table_insert(linecheck, strdup(s), d);
                }
                else {
                    *d = *d + 1 ;
                }
                sb_stemmer_delete(stemmer) ;
            }//d2==null
            *wcp=*wcp+1;
		} //단어별
        g_hash_table_remove_all(linecheck);
		free(_line) ;
		line = 0x0 ;
	}//line

    //non-negative
    FILE * nf = fopen("../data/train.non-negative.csv", "r") ;
    //FILE * nf = fopen("test2.csv", "r") ;
    char * nline = 0x0 ;
    char * nstemmer = "./stemmer \"";
	size_t nr ; 
	size_t nn = 0 ;

    GHashTable * linecheck2 = g_hash_table_new(g_str_hash, g_str_equal) ;
	while (getline(&nline, &nn, nf) >= 0) {
        *nlcp=*nlcp+1;
		char * t ;
		char * _nline = nline ;

		for (t = strtok(nline, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {   
            int j=0;
            char nline[10240];
            for(int i=0; i<strlen(t); i++){
                if(t[i]<65||t[i]>122) continue;
                if(isupper(t[i]))
                    t[j++]=t[i]+32;
                else
                    t[j++]=t[i];
            }
            t[j]='\0';
            struct sb_stemmer * nstemmer ;
            nstemmer = sb_stemmer_new("english", 0x0) ;
            const char * s;
			int * d ;
            s = sb_stemmer_stem(nstemmer, t, strlen(t)) ;
            int * d2 = malloc(sizeof(int)) ;
            d2 = g_hash_table_lookup(linecheck2, strdup(s)) ;
            if(d2==NULL){		
                d = g_hash_table_lookup(ncounter, strdup(s)) ;
                if (d == NULL) {
                    *nwc2p=*nwc2p+1;
                    d = malloc(sizeof(int)) ;
                    *d = 1 ;
                    g_hash_table_insert(ncounter, strdup(s), d) ;
                    g_hash_table_insert(linecheck2, strdup(s), d);
                }
                else {
                    *d = *d + 1 ;
                }
                sb_stemmer_delete(nstemmer) ;
            }
            *nwcp=*nwcp+1;
		}
        g_hash_table_remove_all(linecheck2);
		free(_nline) ;
		nline = 0x0 ;
	}

	//g_hash_table_foreach(counter, print_counter, 0x0) ; //negative를 출력
    //FILE * fp = fopen("model.csv", "w");
    FILE * fp = fopen("../data/model.csv", "w");
    fclose(fp);//file초기화

    g_hash_table_foreach(counter, save_counter, 0x0); //non-negative출력(negative랑 중복되는 것 제외)
    //g_hash_table_foreach(ncounter, print_counter, 0x0) ;
    g_hash_table_foreach(ncounter, save_counter2, 0x0);
    
	//printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "worst"))) ;
	fclose(f) ;
    fclose(nf) ;
    printf("[System] model.csv is made\n");
}
