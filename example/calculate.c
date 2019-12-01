#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>

void print_counter (gpointer key, gpointer value, gpointer userdata)
{
    char * t = key ;
    float * d = value ;

    printf("(%s, %f)\n", t, *d) ;
}

int main () 
{
    
    GHashTable * Negative = g_hash_table_new(g_str_hash, g_str_equal) ;
    GHashTable * NonNegative = g_hash_table_new(g_str_hash, g_str_equal) ;
	FILE * f = fopen("../data/model.csv", "r") ;
    char line [10240]; double neg, nonneg;
    size_t r ;
    size_t n = 0 ;
    
	while (fscanf(f, "%s, %lf, %lf", line, &neg, &nonneg)>=0) {
        printf("%s %lf %lf\n", line, neg, nonneg);
        double * d ;
        d = g_hash_table_lookup(Negative, line) ;
        if (d == NULL) {
            d = malloc(sizeof(double)) ;
            if(neg!=0) *d = log10(neg);
            else *d = 0; // zero prob handling.
            g_hash_table_insert(Negative, strdup(line), d) ;
        }
//        free(d);
//        d  = NULL;
        
        double * d2 ;
        d2 = g_hash_table_lookup(NonNegative, line) ;
        if (d2 == NULL) {
            d2 = malloc(sizeof(double)) ;
            if(nonneg!=0)
            *d2 = log10(nonneg);
            else *d2 = 0;
            g_hash_table_insert(NonNegative, strdup(line), d2) ;
        }
        
//        free(d2);
//        d2  = NULL;
	}//line


	//g_hash_table_foreach(counter, print_counter, 0x0) ; //negative를 출력
    //FILE * fp = fopen("model.csv", "w");
//    FILE * fp = fopen("../data/model_prob.csv", "w");
//    fclose(fp);//file초기화
//    printf("Negative\n");
//    g_hash_table_foreach(Negative, print_counter, 0x0); //non-negative출력(negative랑 중복되는 것 제외)
//    printf("Non-negative\n");
//    //g_hash_table_foreach(ncounter, print_counter, 0x0) ;
//    g_hash_table_foreach(NonNegative, print_counter, 0x0);
//
	//printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "worst"))) ;
	fclose(f) ;
    printf("Calculated\n");
}
