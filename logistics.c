#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "WGraph.h"

//Assign global variable for storing source nodes combinations (stage 3 solution)
SRC_s *asc;

//For referencing city name (stored in city_l array of char array) with list and matrix index in ADT
int index_search(char **l, char key[33], int n){
    for (int i = 0; i < n; i++){
        if(strcmp(l[i], key) == 0){
            return i;
        }
    }
    exit(1);
}

//Recursive binary combination applied for dynamic list (stage 2 and 3 solution)
//Reference from https://www.geeksforgeeks.org/generate-all-the-binary-strings-of-n-bits
//Heavily rewritten to work with dynamic list parsing (only printing before) and made alphabetical order
void all_comb_rec(SRC_s src_set, int no_v, int i) {
   if (i == no_v) {
      int n = pow(2,no_v); //size of n-bit is 2^n
      //Copy content into temp source nodes list
      SRC_s s = calloc(no_v, sizeof(int));
      for (int k = 0; k < no_v; k++) {
         int x = src_set[k];
         s[k] = x;
      }
      //Copy into combination list
      for (int j = 0; j < n; j++)
         if (asc[j][0] == -1) {
            for (int k = 0; k < no_v; k++) {
               int x = src_set[k];
               asc[j][k] = x;
            }
            free(s);
            return;
         }
   }
   // Recursion part into 2 path every iteration
   // This order of 1 before 0, make this follow the same ordering as city_l, which is alphabetical
   src_set[i] = 1; //path 1
   all_comb_rec(src_set, no_v, i + 1);
   src_set[i] = 0; //path 2
   all_comb_rec(src_set, no_v, i + 1);
}

int main() {
   int no_v, no_e;

   //Take city info and create an array for char array for reference
   printf("Enter the number of cities on the distribution network: ");
   scanf("%d", &no_v);
   Graph city_g = newGraph(no_v);

   char **city_l = calloc(no_v, sizeof(*city_l));
   assert(city_l != NULL);

   for (int i = 0; i < no_v; i++) {
      city_l[i] = malloc(33);
      assert(city_l[i] != NULL);
      scanf("%32s", city_l[i]);
   }

   //Alphabetical insertion sort for city_l
   char tmp[33];
   int nx, cur;
   for (nx = 1; nx < no_v; nx++) {
      strcpy(tmp, city_l[nx]);
      cur = nx-1;
      while (cur >= 0 && strcmp(city_l[cur], tmp) > 0) {
         strcpy(city_l[cur+1], city_l[cur]);
         cur--;
      }
      strcpy(city_l[cur+1], tmp);
   }

   // Populate graph with edge info
   int ew;
   char start_v[33] = "";
   char end_v[33] = "";
   printf("Enter the number of roads: ");
   scanf("%d", &no_e);
   for (int i = 0; i < no_e; i++) {

      //Read road info from user input
      printf("Enter the name of a city: ");
      scanf("%32s", start_v);
      printf("Enter the name of a city: ");
      scanf("%32s", end_v);
      printf("Enter the distance: ");
      scanf("%d", &ew);

      //search for start and end node
      int sv_i, ev_i;
      sv_i = index_search(city_l, start_v, no_v);
      ev_i = index_search(city_l, end_v, no_v);

      //create and add edge, with weight
      Edge road;
      road.v = sv_i;
      road.w = ev_i;
      road.weight = ew;
      insertEdge(city_g, road);
   }
   //Read maximum distance allowed for distribution center from user input
   int max_dist;
   printf("\nEnter the required maximum distance: ");
   scanf("%d", &max_dist);

   //Find all single source SPTs, letting each node become source and store in list
   SPT_l spt_l = calloc(no_v, sizeof(SPT));
   for (int i = 0; i < no_v; i++) {
      spt_l[i] = dijkstra(city_g, i, no_v, max_dist);
   }
   freeGraph(city_g);

   // Generate all possible combination of source nodes, stored in asc
   int n = pow(2, no_v);
   asc = calloc(n, sizeof(SRC_s));
   for (int i = 0; i < n; i ++) {
      asc[i] = calloc(no_v, sizeof(int));
      asc[i][0] = -1; //Initialize "blank value"
   }
   SRC_s src_set = calloc(no_v, sizeof(int));
   all_comb_rec(src_set, no_v, 0);
   free(src_set);

   // Test all combinations, find min number of sources
   int min_no_src = no_v;
   for (int c = 0; c < n; c++) {
      int nc = sumNCovered(asc[c], spt_l, max_dist);
      int ns = countSRC(asc[c], no_v);
      if (nc == no_v && ns <= min_no_src)
         min_no_src = ns;
   }
   // Print solutions that uses min number of sources
   for (int c = 0; c < n; c++) {
      int nc = sumNCovered(asc[c], spt_l, max_dist);
      int ns = countSRC(asc[c], no_v);
      if (nc == no_v && ns == min_no_src){
         showSPT(spt_l, asc[c], city_l, max_dist);
         free(asc[c]);
      }
   }
   free(asc);
   for (int i = 0; i < no_v; i++) {
      free(city_l[i]);
      freeSPT(spt_l[i]);
   }
   free(city_l);
}