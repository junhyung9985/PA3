#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <math.h>

void print_counter (gpointer key, gpointer value, gpointer userdata)
{
    char * t = key ;
    int * d = value ;

    printf("(%s, %d, %.3f)\n", t, *d, (*d)*100.0/(*lcp)) ;
}

int main () 
{
    
    GHashTable * Negative = g_hash_table_new(g_str_hash, g_str_equal) ;
    GHashTable * NonNegative = g_hash_table_new(g_str_hash, g_str_equal) ;
	FILE * f = fopen("../data/model.csv", "r") ;
    char line [10240]; float neg, nonneg;
    size_t r ;
    size_t n = 0 ;
    
	while (fscanf(fp, "%s %f %f", line, &neg, &nonneg)>=0) {
        float * d ;
        d = g_hash_table_lookup(Negative, line) ;
        if (d == NULL) {
            d = malloc(sizeof(float)) ;
            *d = log(neg);
            g_hash_table_insert(Negative, strdup(line), d) ;
        }
        
        float * d2 ;
        d2 = g_hash_table_lookup(NonNegative, line) ;
        if (d == NULL) {
            d2 = malloc(sizeof(float)) ;
            *d2 = log(nonneg);
            g_hash_table_insert(NonNegative, strdup(line), d) ;
        }
        
        free(d); free(d2);
	}//line


	//g_hash_table_foreach(counter, print_counter, 0x0) ; //negative를 출력
    //FILE * fp = fopen("model.csv", "w");
//    FILE * fp = fopen("../data/model_prob.csv", "w");
//    fclose(fp);//file초기화

    g_hash_table_foreach(Negative, print_counter, 0x0); //non-negative출력(negative랑 중복되는 것 제외)
    //g_hash_table_foreach(ncounter, print_counter, 0x0) ;
    g_hash_table_foreach(NonNegative, print_counter, 0x0);
    
	//printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "worst"))) ;
	fclose(f) ;
    printf("[System] model.csv is made\n");
}
