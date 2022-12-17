// Weighted Graph ADT interface ... COMP9024 22T3
// Modified by z5274457 for assignment

typedef struct GraphRep *Graph;
typedef struct SPTRep *SPT;

// list of short path trees
typedef SPT *SPT_l;
// list of source nodes combinations
typedef int *SRC_s;
// vertices are char
typedef int Vertex;

// edges are pairs of vertices (end-points) with a weight
typedef struct Edge {
   Vertex v;
   Vertex w;
   int    weight;
} Edge;

Graph newGraph(int);
int   numOfVertices(Graph);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
int   adjacent(Graph, Vertex, Vertex);  // returns weight, or 0 if not adjacent
void  showGraph(Graph);
void  freeGraph(Graph);
SPT   newSPT(int, int, int);
int   dijkstraMinDist(SPT, int*, int, int);
SPT   dijkstra(Graph, int, int, int);
void  showSPT(SPT_l, SRC_s, char**, int);
int   numOfVerticesInSPT(SPT);
int   sumNCovered(SRC_s, SPT_l, int);
int   countSRC(SRC_s, int);
void  freeSPT(SPT);
