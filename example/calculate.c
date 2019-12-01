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

void save_counter(gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
    int * d = value ;
    FILE * fp = fopen("../data/model_prob.csv", "aw");
    int * check = g_hash_table_lookup(nsave, t) ;
    
    if (check == NULL){
        fprintf(fp, "%s, %f, 0\n", t, *d);
    }
    else{
        fprintf(fp, "%s, %f, %f\n", t, *d);
        g_hash_table_remove(nsave, t);
    }
    fclose(fp);
}
void save_counter2(gpointer key, gpointer value, gpointer userdata){
    char * t = key ;
    int * d = value ;
    FILE * fp = fopen("../data/model_prob.csv", "aw");
    fprintf(fp, "%s, 0, %f\n", t, *d);
    fclose(fp);
}


int main () 
{
    
    GHashTable * Negative = g_hash_table_new(g_str_hash, g_str_equal) ;
    GHashTable * NonNegative = g_hash_table_new(g_str_hash, g_str_equal) ;
	FILE * f = fopen("../data/model.csv", "r") ;
    char line [1000000]; double neg, nonneg;
    size_t r ;
    size_t n = 0 ;
    
	while (1) {
        fgets(line, 1000000, f);
         if(feof(f))break;
        char *ptr = strtok(line, ", ");
        char *ptr2 = strtok(NULL, ", ");
        neg = atof(ptr2);
        double *d;
        d = g_hash_table_lookup(Negative, ptr) ;
        if (d == NULL) {
            d = malloc(sizeof(double)) ;
            if(neg!=0) *d = log10(neg);
            else *d = 0; // zero prob handling.
            g_hash_table_insert(Negative, strdup(ptr), d) ;
        }
//        free(d);
//        d  = NULL;
        char *ptr3 = strtok(NULL, ", ");      // 다음 문자열을 잘라서 포인터를 반환
        nonneg = atof(ptr3);
        double * d2 ;
        d2 = g_hash_table_lookup(NonNegative,ptr) ;
        if (d2 == NULL) {
            d2 = malloc(sizeof(double)) ;
            if(nonneg!=0)
            *d2 = log10(nonneg);
            else *d2 = 0;
            g_hash_table_insert(NonNegative, strdup(ptr), d2) ;
        }
      // printf("%s %lf %lf\n", ptr, neg, nonneg);
        
//        free(d2);
//        d2  = NULL;
	}//line


	//g_hash_table_foreach(counter, print_counter, 0x0) ; //negative를 출력
    //FILE * fp = fopen("model.csv", "w");
//    FILE * fp = fopen("../data/model_prob.csv", "w");
//    fclose(fp);//file초기화
//    printf("Negative\n");
 g_hash_table_foreach(Negative, save_counter, 0x0); //non-negative출력(negative랑 중복되는 것 제외)
//    printf("Non-negative\n");
//    //g_hash_table_foreach(ncounter, print_counter, 0x0) ;
  g_hash_table_foreach(NonNegative, save_counter2, 0x0);
//
	//printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "worst"))) ;
	fclose(f) ;
    printf("Calculated\n");
}
