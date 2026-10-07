#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/graph.h"

/* =========================================================
 * graph.c
 * Implementarea grafului direcționat ponderat cu liste de
 * adiacență.
 * ========================================================= */

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru relații
 * ---------------------------------------------------------- */

const char *relation_type_to_str(RelationType type)
{
    /* TODO: returnează șir cu litere mici pentru fiecare enum de relație */
    if(type == WORKS_AT)    return "works_at";
    if(type == FRIEND_OF)   return "friend_of";
    if(type == LOCATED_IN)  return "located_in";
    if(type == PARTICIPATES_IN) return "participates_in";
    return "unknown";
}

RelationType str_to_relation_type(const char *str)
{
    /* TODO: parsează șirul relației la valoarea enum */
    if(strcmp(str, "works_at") == 0) return WORKS_AT;
    if(strcmp(str, "friend_of") == 0)   return FRIEND_OF;
    if(strcmp(str, "located_in") == 0)  return LOCATED_IN;
    if(strcmp(str, "participates_in") == 0) return PARTICIPATES_IN;
    return (RelationType)-1;
}

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Graph *graph_create(int initial_capacity)
{
    /* TODO: alocă structura Graph și tabloul de noduri */
    Graph *g = malloc(sizeof(Graph));
    
    g->nodes = malloc(initial_capacity * sizeof(GraphNode));

    g->size = 0;
    g->capacity = initial_capacity;

    return g;
}

void graph_free(Graph *g)
{
    /* TODO: eliberează șirul de nume al fiecărui nod și lista de muchii,
     *       apoi tabloul și structura */
    for(int i = 0; i < g->size; i++)
    {
        EdgeNode *an = g->nodes[i].edges;
        while(an != NULL)
        {
            EdgeNode *tmp = an;
            an = an->next;
            free(tmp);
        }

        free(g->nodes[i].entity.name);
    }

    free(g->nodes);
    free(g);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int graph_add_node(Graph *g, const char *name, EntityType type)
{
    /* TODO: extinde tabloul dacă e necesar, inițializează noul GraphNode,
     *       returnează id-ul */
    if(g->size == g->capacity)
    {
        g->capacity = g->capacity * 2;
        g->nodes = realloc(g->nodes, g->capacity * sizeof(GraphNode));
    }
    
    int id = g->size;  

    g->nodes[id].entity.name = malloc(strlen(name) + 1);
    strcpy(g->nodes[id].entity.name, name);
    g->nodes[id].entity.type = type;
    g->nodes[id].entity.id = id;
    g->nodes[id].edges = NULL;

    g->size++;
    return id;
}

int graph_add_edge(Graph *g, int src_id, int dest_id,
                   RelationType type, float cost)
{
    /* TODO: alocă EdgeNode, adaugă la finalul listei de muchii a sursei
     *       pentru a păstra ordinea de inserare */
    EdgeNode *an = malloc(sizeof(EdgeNode));
    an->dest_id = dest_id;
    an->type = type;
    an->cost = cost;
    an->next = NULL;

    if(g->nodes[src_id].edges == NULL)
    {
        g->nodes[src_id].edges = an;
        return 0;
    }

    EdgeNode *current = g->nodes[src_id].edges;
    while(current->next != NULL)
        current = current->next;
    current->next = an;

    return 0;
}

/* ----------------------------------------------------------
 * Funcții ajutătoare pentru interogare
 * ---------------------------------------------------------- */

int graph_find_id(const Graph *g, const char *name)
{
    /* TODO: scanare liniară returnând id-ul potrivit sau -1 */
    for(int i = 0; i < g->size; i++)
    {
        if(strcmp(g->nodes[i].entity.name, name) == 0)
            return i; 
    }
    return -1;
}

GraphNode *graph_get_node(const Graph *g, int id)
{
    /* TODO: verifică limitele și returnează pointerul */
    if(id < 0 || id >= g->size)
        return NULL;
    else
        return &g->nodes[id];
}

/* ----------------------------------------------------------
 * Afișare
 * ---------------------------------------------------------- */

void graph_print(const Graph *g)
{
    /* TODO: afișează fiecare nod și lista sa de muchii */
    for(int i = 0; i < g->size; i++)
    {
        printf("%d %s:", g->nodes[i].entity.id, g->nodes[i].entity.name);

        EdgeNode *an = g->nodes[i].edges;
        while(an != NULL)
        {
            printf(" [%d %s %.2f]", an->dest_id, relation_type_to_str(an->type), an->cost);
            an = an->next;
        }
        printf("\n");
    }
}
