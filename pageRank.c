#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>
#include<assert.h>
#include"WGraph.h"

typedef struct GraphRep {
   int **edges;  // adjacency matrix storing positive weights
		 // 0 if nodes not adjacent
   int nV;       // #vertices
   int nE;       // #edges
} GraphRep;

//Check if input is a number by looping throught input string and check each element with isdigit
int is_num(char *inp) {
   int inp_len = strlen(inp);
   int i;
   for (i=0;i<inp_len; i++) {
      if (!isdigit(inp[i])){
         return(0);
      }
   }
   return(1);
}

int main() {
   //Get no. of vertices and create graph
   char usr_inp[10] = "";
   int no_wp;
   printf("Enter the number of webpages: ");
   scanf("%s", usr_inp);
   if (is_num(usr_inp) == 0){
      printf("Done.\n");
      exit(0);
   }
   no_wp = atoi(usr_inp);
   Graph graph_web = newGraph(no_wp);

   //Take user input and add edges accordingly
   int page;
   int link;
   char usr_inp_p[10];
   char usr_inp_l[10];
   Edge new_edge;
   new_edge.weight = 1;

   while(1){

      //Starting node input
      printf("Enter a webpage: ");
      scanf("%s", usr_inp_p);
      if (is_num(usr_inp_p) == 0) break; //Check for non-numeric input to break loop
      page = atoi(usr_inp_p);

      //End node input
      printf("Enter a link on webpage %d: ", page);
      scanf("%s", usr_inp_l);
      if (is_num(usr_inp_l) == 0) break; //Check for non-numeric input to break loop
      link = atoi(usr_inp_l);
      
      //Adding new edges 
      new_edge.v = page;
      new_edge.w = link;
      insertEdge(graph_web, new_edge);
   }

   printf("Done.\n");
   
   //Pages Scoring
   int nv = 0;
   nv = numOfVertices(graph_web);
   //printf("no. of vertex done!\n");
   int **score;
   //printf("scoring matrix done!\n");

   score = malloc(nv * sizeof(int *));
   //printf("row allocation done!\n");
   assert(score != NULL);
   // allocate memory for each column and initialise with 0
   for (int i = 0; i < nv; i++) {
      score[i] = calloc(3, sizeof(int));
      assert(score[i] != NULL);
   }
   //printf("column allocation done!\n");

   //Filling in scoring matrix based on no. of inbound links and page's outbound links' inbound links
   int r, c, k;
   for (r = 0; r < nv; r++) {
      score[r][0] = r;
   	for (c = 0; c < nv; c++) {
   		if (graph_web->edges[r][c] != 0) {
   			//printf("inbound link for %d\n", i);
   			score[c][1]++;
            for (k = 0; k < nv; k++) {
               if (graph_web->edges[k][c] != 0) {
                  //printf("outbound score for %d " , i);
                  //printf("to %d\n", j);
                  score[r][2]++;
               }
            }
   		}
   	}
   }
   //Free orignal graph after filling in scoring matrix
   freeGraph(graph_web);

   //Dsc insertion sort, modifed for 2D pointer-pointer array to sort row by columns
   int *tmp = calloc(3, sizeof(int));
   int i;
   for (i = 1; i < nv; i++) {
      tmp = score[i];                           // for this element ...
      int j = i-1;
      while (j >= 0 && (score[j][1] < tmp[1] || (score[j][1] == tmp[1] && score[j][2] < tmp[2]))) {  // ... work down the ordered list
         score[j+1] = score[j];                 // ... moving elements up
         j--;
      }
      score[j+1] = &*tmp;                         // and insert in correct position
   } 

   //Print page ranking
   printf("\nWebpage ranking:\n");
   for (i = 0; i < nv; i++) {
      printf("%d has %d inbound links and scores %d on outbound links.\n", score[i][0], score[i][1], score[i][2]);
      free(score[i]);
   }
   free(score);
}
