#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <ctype.h>
#include "../include/libstemmer.h"
void
print_counter (gpointer key, gpointer value, gpointer userdata) {
   char * t = key ;
   int * d = value ;
   printf("(%s, %d)\n", t, *d) ;
}
int
main () {
   FILE * f = fopen("../data/train.negative.csv", "r") ;
   FILE *fp=NULL;
   GHashTable * counter = g_hash_table_new(g_str_hash, g_str_equal) ;
   char * line = 0x0 ;
   char * stemmer = "./stemmer \"";
   size_t r ; 
   size_t n = 0 ; 
   while (getline(&line, &n, f) >= 0) {
      char * t ; 
      char * _line = line ;
      for (t = strtok(line, " \n\t") ; t != 0x0 ; t = strtok(0x0, " \n\t")) {
         int j=0;
         char line[10240];
         for (int i=0; i<strlen(t); i++) {
            if(t[i]<65||t[i]>122) continue;
            if(isupper(t[i]))
                                t[j++]=t[i]+32; else
                                t[j++]=t[i];
         }   
         t[j]='\0';
         struct sb_stemmer * stemmer ;
         stemmer = sb_stemmer_new("english", 0x0) ;
         const char * s;
         int * d ; 
         s = sb_stemmer_stem(stemmer, t, strlen(t)) ;
         d = g_hash_table_lookup(counter, s) ;
         if (d == NULL) {
            d = malloc(sizeof(int)) ;
            *d = 1 ; 
            g_hash_table_insert(counter, strdup(s), d) ;
         } else {
            *d = *d + 1 ; 
         }   
         sb_stemmer_delete(stemmer) ;
      }   
      free(_line) ;
      line = 0x0 ;
   }   
   g_hash_table_foreach(counter, print_counter, 0x0) ;
   printf("worst: %d\n", *((int *) g_hash_table_lookup(counter, "worst"))) ;
   fclose(f) ;
   return 0;
}