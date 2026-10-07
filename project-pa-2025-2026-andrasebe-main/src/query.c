#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <float.h>

#include "../include/query.h"
#include "../include/heap.h"

/* =========================================================
 * query.c
 * Parsarea și dispatcharea interogărilor de graf:
 *   EXISTS, EDGE, NEIGHBORS, PATH (BFS), DIJKSTRA
 * ========================================================= */

/* ----------------------------------------------------------
 * Helper de parsare
 * ---------------------------------------------------------- */

QueryType parse_query_type(const char *line)
{
    /* TODO: potrivește cuvântul cheie de la începutul liniei */
    if(strncmp(line, "EXISTS", 6) == 0) return Q_EXISTS;
    if(strncmp(line, "EDGE", 4) == 0)   return Q_EDGE;
    if(strncmp(line, "NEIGHBORS", 9) == 0)  return Q_NEIGHBORS;
    if(strncmp(line, "PATH", 4) == 0)   return Q_PATH;
    if(strncmp(line, "DIJKSTRA", 8) == 0)   return Q_DIJKSTRA;
    return Q_UNKNOWN;
}

/* ----------------------------------------------------------
 * EXISTS
 * ---------------------------------------------------------- */

void process_exists(const BST *tree, const char *name)
{
    /* TODO: caută în BST, afișează DA/NU */
    GraphNode *node = bst_search(tree, name);
    printf("EXISTS %s: %s\n", name, node ? "DA" : "NU");
}

/* ----------------------------------------------------------
 * EDGE
 * ---------------------------------------------------------- */

void process_edge(const Graph *g, const BST *tree,
                  const char *src_name, const char *dest_name)
{
    /* TODO: găsește ambele noduri via BST, scanează lista de muchii
     *       a sursei pentru destinație */
    (void)g;
    GraphNode *src = bst_search(tree, src_name);
    GraphNode *dist = bst_search(tree, dest_name);
    
    if(!src || !dist)
    {
        printf("EDGE %s %s: NU\n", src_name, dest_name);
        return;
    }

    EdgeNode *an = src->edges;
    while(an != NULL)
    {
        if(an->dest_id == dist->entity.id)
        {
            printf("EDGE %s %s: DA\n", src_name, dest_name);
            return;
        }
        an = an->next;
    }
    printf("EDGE %s %s: NU\n", src_name, dest_name);
}

/* ----------------------------------------------------------
 * NEIGHBORS
 * ---------------------------------------------------------- */

void process_neighbors(const Graph *g, const BST *tree,
                       const char *name)
{
    /* TODO: găsește nodul, iterează lista de muchii, afișează
     *       numele vecinilor; dacă nu există muchii de ieșire
     *       afișează "NEIGHBORS <name>: NULL" */
   GraphNode *node = bst_search(tree, name);
   if(!node || !node->edges)
   {
        printf("NEIGHBORS %s: NULL\n", name);
        return;
   }
   printf("NEIGHBORS %s:", name);

   EdgeNode *an = node->edges;
   while(an != NULL)
   {
        GraphNode *dest = graph_get_node(g, an->dest_id);
        if(dest != NULL)
            printf(" %s", dest->entity.name);
        an = an->next;
   }
   printf("\n");
}

/* ----------------------------------------------------------
 * PATH (BFS)
 * ---------------------------------------------------------- */
void print_path(const Graph *g, const int *parent, int dest_id, const char *src_name, const char *dest_name)
{
    int *path = malloc(g->size * sizeof(int));
    int len = 0, cur = dest_id;

    while(cur != -1)
    {
        path[len++] = cur;
        cur = parent[cur];
    }

    printf("PATH %s %s:", src_name, dest_name);
    for(int i = len - 1; i >= 0; i--)
    {
        printf("%s%s", (i == len - 1) ? " " : " -> ", g->nodes[path[i]].entity.name);
    }
    printf("\n");
    free(path);
}
void process_path_bfs(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: BFS de la src la dest, reconstruiește și afișează calea;
     *       dacă nu există cale afișează "PATH <src> <dest>: NU" */
    GraphNode *src = bst_search(tree, src_name);
    GraphNode *dest = bst_search(tree, dest_name);
    if(!src || !dest){
        printf("PATH %s %s: NU\n", src_name, dest_name);
        return;
    }
    int *visited = calloc(g->size, sizeof(int));
    int *parent = malloc(g->size * sizeof(int));
    int *coada = malloc(g->size * sizeof(int));
    int front = 0, rear = 0;

    for(int i = 0; i < g->size; i++)    parent[i] = -1; 

    visited[src->entity.id] = 1;
    coada[rear++] = src->entity.id;

    while(front < rear){
        int cur = coada[front++];
        if(cur == dest->entity.id)   break;

        EdgeNode *an = graph_get_node(g, cur)->edges;
        while(an != NULL){
            if(!visited[an->dest_id]){
                visited[an->dest_id] = 1;
                parent[an->dest_id] = cur;
                coada[rear++] = an->dest_id;
            }
            an = an->next;
        }
    }

    if(!visited[dest->entity.id])
        printf("PATH %s %s: NU\n", src_name, dest_name);
    else
        print_path(g, parent, dest->entity.id, src_name, dest_name);
    
    free(visited);  free(parent);   free(coada);
}

/* ----------------------------------------------------------
 * DIJKSTRA
 * ---------------------------------------------------------- */
void print_dijkstra_path(const Graph *g, const int *parinte, int dest_id, const char *src_name, const char *dest_name, float cost)
{
    int *path = malloc(g->size * sizeof(int));
    int len = 0;

    for(int cur = dest_id; cur != -1; cur = parinte[cur])
        path[len++] = cur;

    printf("DIJKSTRA %s %s: COST = %.2f; DRUM =", src_name, dest_name, cost);
    for(int i = len - 1; i >= 0; i--)
    {
        if(i != len - 1)    printf(" ->");
        printf(" %s", g->nodes[path[i]].entity.name);
    }
    printf("\n");
    free(path);
}

void process_dijkstra(const Graph *g, const BST *tree,
                      const char *src_name, const char *dest_name)
{
    /* TODO: Dijkstra cu min-heap, afișează costul și calea;
     *       dacă nu există cale afișează "DIJKSTRA <src> <dest>: NU" */
    GraphNode *src = bst_search(tree, src_name);
    GraphNode *dest = bst_search(tree, dest_name);
    if(!src || !dest){
        printf("DIJKSTRA %s %s: NU\n", src_name, dest_name);
        return;
    }
    
    float *dist = malloc(g->size * sizeof(float));
    int *parinte = malloc(g->size * sizeof(int));

    for(int i = 0; i < g->size; i++){
        dist[i] = FLT_MAX;
        parinte[i] = -1;
    }
    dist[src->entity.id] = 0.0f;

    MinHeap *heap = heap_create(g->size);
    heap_push(heap, src->entity.id, 0.0f);

    while(!heap_is_empty(heap)){
        HeapNode curent = heap_pop(heap);
        int nod_curent = curent.node_id;

        if(curent.dist > dist[nod_curent])
                   continue;
        
        for(EdgeNode *muchie = g->nodes[nod_curent].edges; muchie; muchie = muchie->next){
            float dist_nou = dist[nod_curent] + muchie->cost;
            if(dist_nou < dist[muchie->dest_id]){
                dist[muchie->dest_id] = dist_nou;
                parinte[muchie->dest_id] = nod_curent;
                heap_push(heap, muchie->dest_id, dist_nou);
            }
        }
    }
    heap_free(heap);

    if(dist[dest->entity.id] == FLT_MAX)
        printf("DIJKSTRA %s %s: NU\n", src_name, dest_name);
    else
        print_dijkstra_path(g, parinte, dest->entity.id, src_name, dest_name, dist[dest->entity.id]);

    free(dist); free(parinte);
}


/* ----------------------------------------------------------
 * Dispatcher batch
 * ---------------------------------------------------------- */

void process_all_queries(Queue *q, const Graph *g, const BST *tree)
{
    /* TODO: extrage fiecare linie, parsează tipul, dispatchează
     *       la handlerul corespunzător */
    while(!queue_is_empty(q)){
        char *linie = queue_dequeue(q);
        if(!linie)  continue;

        char tmp[512];
        strncpy(tmp, linie, sizeof(tmp) - 1);
        tmp[sizeof(tmp) - 1] = '\0';

        QueryType tip = parse_query_type(tmp);
        strtok(tmp, " ");
        char *src = strtok(NULL, " ");
        char *dest = strtok(NULL, " ");

        if(tip == Q_EXISTS) process_exists(tree, src);
        else if(tip == Q_EDGE)  process_edge(g, tree, src, dest);
        else if(tip ==  Q_NEIGHBORS)    process_neighbors(g, tree, src);
        else if(tip == Q_PATH)  process_path_bfs(g, tree, src, dest);
        else if(tip == Q_DIJKSTRA)  process_dijkstra(g, tree, src, dest);

        free(linie);
    }
}
