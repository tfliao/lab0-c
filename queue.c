/*
 * Code for basic C skills diagnostic.
 * Developed for courses 15-213/18-213/15-513 by R. E. Bryant, 2017
 * Modified to store strings, 2018
 */

/*
 * This program implements a queue supporting both FIFO and LIFO
 * operations.
 *
 * It uses a singly-linked list to represent the set of queue elements
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "harness.h"
#include "queue.h"

static list_ele_t *alloc_ele(char *s);
static void free_ele(list_ele_t *node);

/*
  Create empty queue.
  Return NULL if could not allocate space.
*/
queue_t *q_new()
{
    queue_t *q = malloc(sizeof(queue_t));
    if (q == NULL) {
        goto end;
    }
    q->head = NULL;
    q->size = 0;
    q->tail = NULL;
end:
    return q;
}

/* Free all storage used by queue */
void q_free(queue_t *q)
{
    if (q != NULL) {
        list_ele_t *curr = q->head;
        list_ele_t *next = NULL;
        while (curr != NULL) {
            next = curr->next;
            free_ele(curr);
            curr = next;
        }
    }
    free(q);
}

/*
  Attempt to insert element at head of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_head(queue_t *q, char *s)
{
    bool success = false;
    list_ele_t *newh = NULL;

    if (q == NULL) {
        goto end;
    }
    newh = alloc_ele(s);
    if (newh == NULL) {
        goto end;
    }
    if (q->size == 0) {
        q->head = q->tail = newh;
    } else {
        newh->next = q->head;
        q->head = newh;
    }
    q->size++;
    success = true;
end:
    return success;
}

/*
  Attempt to insert element at tail of queue.
  Return true if successful.
  Return false if q is NULL or could not allocate space.
  Argument s points to the string to be stored.
  The function must explicitly allocate space and copy the string into it.
 */
bool q_insert_tail(queue_t *q, char *s)
{
    bool success = false;
    list_ele_t *node = NULL;

    if (q == NULL) {
        goto end;
    }
    node = alloc_ele(s);
    if (node == NULL) {
        goto end;
    }
    if (q->size == 0) {
        // queue is empty, head & tail should be NULL
        q->head = q->tail = node;
    } else {
        q->tail->next = node;
        q->tail = node;
    }
    q->size++;
    success = true;
end:
    return success;
}

/*
  Attempt to remove element from head of queue.
  Return true if successful.
  Return false if queue is NULL or empty.
  If sp is non-NULL and an element is removed, copy the removed string to *sp
  (up to a maximum of bufsize-1 characters, plus a null terminator.)
  The space used by the list element and the string should be freed.
*/
bool q_remove_head(queue_t *q, char *sp, size_t bufsize)
{
    bool success = false;
    if (q == NULL || q->size == 0) {
        goto end;
    }
    list_ele_t *node = q->head;
    if (sp != NULL) {
        strncpy(sp, node->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    q->head = q->head->next;
    free_ele(node);
    q->size--;
    if (q->size == 0) {
        q->tail = NULL;
    }
    success = true;
end:
    return success;
}

/*
  Return number of elements in queue.
  Return 0 if q is NULL or empty
 */
int q_size(queue_t *q)
{
    if (q == NULL) {
        return 0;
    } else {
        return q->size;
    }
}

/*
  Reverse elements in queue
  No effect if q is NULL or empty
  This function should not allocate or free any list elements
  (e.g., by calling q_insert_head, q_insert_tail, or q_remove_head).
  It should rearrange the existing ones.
 */
void q_reverse(queue_t *q)
{
    if (q == NULL || q->size <= 1) {
        return;
    }
    q->tail = q->head;

    list_ele_t *last = NULL;
    list_ele_t *curr = q->head;
    list_ele_t *next = NULL;
    while (curr != NULL) {
        next = curr->next;
        curr->next = last;

        last = curr;
        curr = next;
    }
    q->head = last;
}

/*
  Attempt to allocate list_ele_t with givne string as value.
  Return NULL if we could not allocate space for either list_ele_t or string.
  Return pointer to list_ele_t if success.
 */
static list_ele_t *alloc_ele(char *s)
{
    list_ele_t *e = NULL;

    e = malloc(sizeof(list_ele_t));
    if (e == NULL) {
        goto fail_ele;
    }
    // XXX: handle s is NULL
    e->value = malloc(strlen(s) + 1);
    if (e->value == NULL) {
        goto fail_data;
    }
    // We allocate e->value according to length of s,
    //   so it's safe to simply use strcpy here.
    strcpy(e->value, s);
    e->next = NULL;
    return e;

fail_data:
    free(e);
fail_ele:
    return NULL;
}


/*
  Free everything belong to node
 */
static void free_ele(list_ele_t *node)
{
    if (node != NULL) {
        free(node->value);
    }
    free(node);
}
