#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/queue.h"

/* =========================================================
 * queue.c
 * Coadă FIFO susținută de o listă simplu înlănțuită.
 * Stochează copii alocate pe heap ale șirurilor.
 * ========================================================= */

/* ----------------------------------------------------------
 * Ciclul de viață
 * ---------------------------------------------------------- */

Queue *queue_create(void)
{
    /* TODO: alocă și inițializează cu zero o structură Queue */
    Queue *q = malloc(sizeof(Queue));
    q->front = NULL;
    q->rear = NULL;
    q->size = 0;
    return q;
}

void queue_free(Queue *q)
{
    /* TODO: golește coada eliberând fiecare nod și datele sale */
    while(q->front != NULL)
    {
        QueueNode *tmp = q->front;
        q->front = q->front->next;
        free(tmp->data);
        free(tmp);
    }
    free(q);
}

/* ----------------------------------------------------------
 * Mutație
 * ---------------------------------------------------------- */

int queue_enqueue(Queue *q, const char *data)
{
    /* TODO: duplică data, alocă QueueNode, adaugă la capătul rear */
    QueueNode *node = malloc(sizeof(QueueNode));

    node->data = malloc(strlen(data) + 1);
    strcpy(node->data, data);
    node->next = NULL;

    if(q->rear == NULL)
    {
        q->front = node;
        q->rear = node;
        q->size++;
        return 0;
    }

    q->rear->next = node;
    q->rear = node;
    q->size++;
    return 0;
}

char *queue_dequeue(Queue *q)
{
    /* TODO: elimină nodul din față, returnează datele sale (apelantul
     *       eliberează) */
    if(q->front == NULL)    return NULL;

    QueueNode *tmp = q->front;
    char *data = tmp->data;

    q->front = q->front->next;

    if(q->front ==  NULL)   q->rear = NULL;

    q->size--;
    free(tmp);
    return data;
}

/* ----------------------------------------------------------
 * Inspecție
 * ---------------------------------------------------------- */

int queue_is_empty(const Queue *q)
{
    /* TODO: returnează 1 când size == 0 */
    if(q->size == 0)
        return 1;
    else
        return 0;
}

void queue_print(const Queue *q)
{
    /* TODO: iterează de la front la rear, afișează fiecare șir de date */
    QueueNode *current = q->front;
    while(current != NULL)
    {
        printf("%s\n", current->data);
        current = current->next;
    }
}
