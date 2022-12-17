// Weighted Directed Graph ADT
// Adjacency Matrix Representation ... COMP9024 22T3
// Modified by z5274457 for assignment
#include "WGraph.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct GraphRep {
   int **edges;  // adjacency matrix storing positive weights
   // 0 if nodes not adjacent
   int nV;       // #vertices
   int nE;       // #edges
} GraphRep;

// For storing shortest path tree in a 2 by n matrix
typedef struct SPTRep {
   int **tree;   // adjacency matrix storing path from src to dest (row), column 0 is previous node, column 1 is dist
   int source;   // source node index
   int nP;       // #shortest path to other nodes, for determining minimal solution
   int tCost;    // total cost
   int nV;       // #nodes
} SPTRep;


Graph newGraph(int V) {
   assert(V >= 0);
   int i;

   Graph g = malloc(sizeof(GraphRep));
   assert(g != NULL);
   g->nV = V;
   g->nE = 0;

   // allocate memory for each row
   g->edges = malloc(V * sizeof(int *));
   assert(g->edges != NULL);
   // allocate memory for each column and initialise with maximum distance + 1
   for (i = 0; i < V; i++) {
      g->edges[i] = calloc(V, sizeof(int));
      assert(g->edges[i] != NULL);
      for (int j = 0; j < V; j++)
         g->edges[i][j] = 999999999;
   }
   return g;
}

SPT newSPT(int src, int V, int max_dist) {
   assert(V >= 0);

   SPT s = malloc(sizeof(SPTRep));
   assert(s != NULL);
   s->source = src;
   s->nP = 0;
   s->tCost = 0;
   s->nV = V;

   // allocate memory for each row
   s->tree = malloc(V * sizeof(int *));
   assert(s->tree != NULL);
   // allocate memory for each column and initialise with 0
   for (int i = 0; i < V; i++) {
      s->tree[i] = calloc(2, sizeof(int));
      assert(s->tree[i] != NULL);
      s->tree[i][0] = -1; //intialize all predecessors to -1
      s->tree[i][1] = max_dist+1; //initialize all distance to maximum distance + 1
   }
   return s;
}

int numOfVerticesInSPT(SPT s) {
   return s->nP;
}

int numOfVertices(Graph g) {
   return g->nV;
}

// check if vertex is valid in a graph
int validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}

void insertEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (g->edges[e.v][e.w] >= 999999999) {   // edge e not in graph if more than max_dist
      g->edges[e.v][e.w] = e.weight;
      g->nE++;
   }
}

void removeEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (g->edges[e.v][e.w] != 0) {   // edge e in graph
      g->edges[e.v][e.w] = 0;
      g->nE--;
   }
}

int adjacent(Graph g, Vertex v, Vertex w) {
   assert(g != NULL && validV(g,v) && validV(g,w));

   return g->edges[v][w];
}

void showGraph(Graph g) {
   assert(g != NULL);
   int i, j;

   printf("Number of vertices: %d\n", g->nV);
   printf("Number of edges: %d\n", g->nE);
   for (i = 0; i < g->nV; i++)
      for (j = 0; j < g->nV; j++)
         if (g->edges[i][j] != 0)
            printf("Edge %d - %d: %d\n", i, j, g->edges[i][j]);
}

void freeGraph(Graph g) {
   assert(g != NULL);

   int i;
   for (i = 0; i < g->nV; i++)
      free(g->edges[i]);
   free(g->edges);
   free(g);
}

void freeSPT(SPT s) {
   assert(s != NULL);
   int i;
   for (i = 0; i < s->nV; i++)
      free(s->tree[i]);
   free(s->tree);
   free(s);
}

// Find next minimum node to process in dijkstra algorithm
// Part of dijkstra algorithm below
int dijkstraMinDist(SPT spt, int *spt_set, int no_v, int max_dist) {
   int min = max_dist, min_index = no_v;
   for (int i = 0; i < no_v; i++)
      //if node hasn't been processed and has the shortest distance of the set
      if (spt_set[i] == 0 && spt->tree[i][1] <= min) {
         min = spt->tree[i][1];
         min_index = i;
      }
   return min_index; //return index of the minimum node
}

// Find shortest path to every other nodes from a single source
// Referenced from https://www.geeksforgeeks.org/dijkstras-shortest-path-algorithm-greedy-algo-7
// Heavily rewritten to actually store path and work with dynamic list
SPT dijkstra(Graph g, int src, int no_v, int max_dist) {
   //Intialize data structure,
   //spt: record tree
   //prev_node: for determining path for each dest later
   //spt_set: check (0 for no, 1 for yes) whether a node has been processed
   SPT spt = newSPT(src, no_v, max_dist);
   int prev_node = src;
   int *spt_set = calloc(no_v, sizeof(int));

   spt->tree[src][1] = 0;

   // Find shortest path for all vertices
   for (int i = 0; i < no_v; i++) {
      int u = dijkstraMinDist(spt, spt_set, no_v, max_dist);
      if (u == no_v){
         //printf("no more road\n");
         break;
      }

      spt_set[u] = 1;                  // Check node as processed
//      spt->tree[u][0] = prev_node;     // Record previous node
      spt->nP ++;                      // Update SPT aggregate values
      spt->tCost += spt->tree[u][1];
//      prev_node = u;                   // Set this node as previous node for next iteration


      // Update all neighbor node distance with current node distance + u->v edge weight in graph
      for (int v = 0; v < no_v; v++)
         if (spt_set[v] == 0 &&
         g->edges[u][v] < 999999999 &&
         spt->tree[u][1] <= max_dist &&
         spt->tree[u][1] + g->edges[u][v] < spt->tree[v][1]
         ) {
            spt->tree[v][1] = spt->tree[u][1] + g->edges[u][v];
            spt->tree[v][0] = u;     // Record previous node
         }
   }
   free(spt_set);
   return spt;
}

// Count number nodes (destination) covered in a solution (used to determine minimal solution)
int sumNCovered(SRC_s src_set, SPT_l sptl, int max_dist) {
   int total = 0;
   int no_v = sptl[0]->nV;
   int *destCheck = calloc(no_v, sizeof(int));
   for (int s = 0; s < no_v; s++) {
      if (src_set[s] == 1) {
         for (int dest = 0; dest < no_v; dest++)
            if (sptl[s]->tree[dest][1] <= max_dist && destCheck[dest] == 0) {
               destCheck[dest] = 1;
               total++;
            }
      }
   }
   free(destCheck);
   return total;
}

// Count number of source nodes in a solution (used to determine minimal solution)
int countSRC(SRC_s src_set, int no_v) {
   int total = 0;
   for (int s = 0; s < no_v; s++)
      if (src_set[s] == 1)
         total++;
   return total;
}

// Print result from a combination of source node
void showSPT(SPT_l spt_l, SRC_s src_set, char **l, int max_dist) {
   printf("\nHubs:");
   int nv = spt_l[0]->nV;
   int ns = countSRC(src_set, nv);
   int sc = 0;
   for (int s = 0; s < nv; s++) {
      if (src_set[s] == 1) {
         sc++;
         printf(" %s", l[s]);
         if (sc < ns)
            printf(",");
      }
   }
   printf("\nRoutes:\n");

   // Looping through each destination
   for (int d = 0; d < nv; d++) {
      int min_d = max_dist;
      int best_src = nv;
      // Find min path from each source in solution
      for (int s = 0; s < nv; s++) {
         if (src_set[s] == 1)
            if (spt_l[s]->tree[d][1] <= min_d) {
               min_d = spt_l[s]->tree[d][1];
               best_src = s;
            }
      }
      printf("%s:", l[d]);

      if (best_src != d) {
         // Initialize path with index value of nv, so we know when printing that its blank space
         int *path = calloc(nv, sizeof(int));
         for (int j = 0; j < nv; j++)
            path[j] = nv;

         // Record node in path from dest to source
         int prev = spt_l[best_src]->tree[d][0];
         int x = 0;
         while (prev != spt_l[best_src]->source) {
            path[x] = prev;
            prev = spt_l[best_src]->tree[prev][0];
            x++;
         }
         path[x] = spt_l[best_src]->source;

         // Print backward to get src to dest path
//         for (int i = 0; i < nv; i++)
//            printf("%d\n", path[i]);
         int y = nv - 1;
         while (path[y] == nv)
            y--; //Skipping through blank at the end of list
         for (int yy = y; yy >= 0; yy--)
            printf(" %s -", l[path[yy]]);
         free(path);
      }
      printf(" %s %d\n", l[d], spt_l[best_src]->tree[d][1]);
   }
}