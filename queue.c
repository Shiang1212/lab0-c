#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "queue.h"

/* Notice: sometimes, Cppcheck would find the potential NULL pointer bugs,
 * but some of them cannot occur. You can suppress them by adding the
 * following line.
 *   cppcheck-suppress nullPointer
 */


/* Create an empty queue */
struct list_head *q_new()
{
    struct list_head *head = malloc(sizeof(struct list_head));
    if(!head)
        return NULL;
    INIT_LIST_HEAD(head); // I saw this function in someone's code
    return head;
}

/* Free all storage used by queue */
void q_free(struct list_head *head) 
{
    if (!head)
        return;
    struct list_head *temp = head;
    element_t *entry, *safe;
    list_for_each_entry_safe (entry, safe, temp, list)
    {
        q_release_element(entry);
    }
    free(head);

    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if(!new)
        return false;
    int len = strlen(s);
    new->value = malloc(len + 1);
    if(!new->value)
    {
        free(new);
        return false;
    }
    memcpy(new->value, s, len);
    new->value[len] = '\0';
    list_add(&new->list, head);

    return true;
}

/* Insert an element at tail of queue */
bool q_insert_tail(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if(!new)
        return false;
    int len = strlen(s);
    new->value = malloc(len + 1);
    if(!new->value)
    {
        free(new);
        return false;
    }
    memcpy(new->value, s, len);
    new->value[len] = '\0';
    list_add_tail(&new->list, head);

    return true;
}

/* Remove an element from head of queue */
element_t *q_remove_head(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *remove = list_entry(head->next, element_t, list);
    if (bufsize)
    {
        strncpy(sp, remove->value, bufsize - 1);
        sp[bufsize - 1] = '\0';
    }
    list_del(&remove->list);
    return remove;
}

/* Remove an element from tail of queue */
element_t *q_remove_tail(struct list_head *head, char *sp, size_t bufsize)
{
    if (!head || list_empty(head))
        return NULL;
    element_t *remove = list_entry(head->prev, element_t, list);
    if (bufsize) {
        strncpy(sp, remove->value, bufsize - 1);
;       sp[bufsize - 1] = '\0';
    }
    list_del(&remove->list);
    return remove;
}

/* Return number of elements in queue */
int q_size(struct list_head *head)
{
    if (!head || list_empty(head))
        return 0;
    int cnt = 0;
    struct list_head *temp = head;
    
    for(temp = temp->next; temp != head; temp = temp->next)
        cnt++;
    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head)
        return false;
    struct list_head *fast = head->next->next, *slow = head->next;
    while (!(fast == head->prev) && !(fast == head))
    {
        fast = fast->next->next;
        if (fast == head->prev || fast == head) {
            break;
        }
        slow = slow->next;
    }
    element_t *node = list_entry(slow->next, element_t, list);
    list_del(&node->list);
    q_release_element(node);
    return true;
    // https://leetcode.com/problems/delete-the-middle-node-of-a-linked-list/
 
}

/* Delete all nodes that have duplicate string */
bool q_delete_dup(struct list_head *head)
{
    if (!head)
        return false;
    if (list_empty(head) || list_is_singular(head))
        return true;
    element_t *node, *safe;
    bool flag = 0;
    list_for_each_entry_safe (node, safe, head, list) {
        char *str = list_entry(node->list.next, element_t, list)->value;
        if (node->list.next != head && !strcmp(str, node->value)) {
            list_del(&node->list);
            q_release_element(node);
            flag = 1;
        } else if (flag) {
            list_del(&node->list);
            q_release_element(node);
            flag = 0;
        }
    }
    return true;
    // https://leetcode.com/problems/remove-duplicates-from-sorted-list-ii/
}

/* Swap every two adjacent nodes */
void q_swap(struct list_head *head)
{
    
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head) 
{
    
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
}

/* Sort elements of queue in ascending/descending order */
void q_sort(struct list_head *head, bool descend) {}

/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    return 0;
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    return 0;
}
