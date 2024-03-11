#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    if (!head)
        return NULL;
    INIT_LIST_HEAD(head);
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
        q_release_element(entry);
    free(head);

    return;
}

/* Insert an element at head of queue */
bool q_insert_head(struct list_head *head, char *s)
{
    if (!head)
        return false;
    element_t *new = malloc(sizeof(element_t));
    if (!new)
        return false;
    int len = strlen(s);
    new->value = malloc(len + 1);
    if (!new->value) {
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
    if (!new)
        return false;
    int len = strlen(s);
    new->value = malloc(len + 1);
    if (!new->value) {
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
    if (bufsize) {
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
        sp[bufsize - 1] = '\0';
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
    for (temp = temp->next; temp != head; temp = temp->next)
        cnt++;
    return cnt;
}

/* Delete the middle node in queue */
bool q_delete_mid(struct list_head *head)
{
    if (!head)
        return false;
    struct list_head *fast = head->next->next, *slow = head->next;
    while (!(fast == head->prev) && !(fast == head)) {
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
    if (!head || list_is_singular(head))
        return;
    struct list_head *node, *next;
    for (node = head->next, next = node->next; node != head && next != head;
         node = node->next, next = node->next) {
        list_del(next);
        next->prev = node->prev;
        next->next = node;
        node->prev->next = next;
        node->prev = next;
    }
    // https://leetcode.com/problems/swap-nodes-in-pairs/
}

/* Reverse elements in queue */
void q_reverse(struct list_head *head)
{
    struct list_head *li, *sa;
    list_for_each_safe (li, sa, head)
        list_move(li, head);
}

/* Reverse the nodes of the list k at a time */
void q_reverseK(struct list_head *head, int k)
{
    // https://leetcode.com/problems/reverse-nodes-in-k-group/
    if (!head || list_is_singular(head))
        return;
    struct list_head tmp_head;
    INIT_LIST_HEAD(&tmp_head);
    int ct = 0;
    int size = q_size(head);
    struct list_head *node, *safe;
    list_for_each_safe (node, safe, head) {
        ct += 1;
        if (ct % k == 0 || ct == size) {
            list_cut_position(&tmp_head, head, node);
            q_reverse(&tmp_head);
            list_splice_tail(&tmp_head, head);
        }
        if (ct == size)
            break;
    }
}

/* Sort elements of queue in ascending/descending order */


struct list_head *merge_two_nodes(struct list_head *left,
                                  struct list_head *right)
{
    struct list_head *new_head = NULL, **indirect = &new_head, **iter = NULL;
    for (; left && right; *iter = (*iter)->next) {
        iter = strcmp(list_entry(left, element_t, list)->value,
                      list_entry(right, element_t, list)->value) >= 0
                   ? &right
                   : &left;
        *indirect = *iter;
        indirect = &(*indirect)->next;
    }
    *indirect = (struct list_head *) ((uint64_t) left | (uint64_t) right);
    return new_head;
}

struct list_head *merge_divide(struct list_head *head)
{
    if (!head || !head->next)
        return head;
    struct list_head *rabbit = head, *turtle = head, *middle;

    for (; rabbit && rabbit->next;
         rabbit = rabbit->next->next, turtle = turtle->next)
        ;
    middle = turtle;
    // cut off the link
    turtle->prev->next = NULL;
    struct list_head *left = merge_divide(head);
    struct list_head *right = merge_divide(middle);

    return merge_two_nodes(left, right);
}


void q_sort(struct list_head *head, bool descend)
{
    if (!head || list_empty(head))
        return;
    // cut off the link
    head->prev->next = NULL;
    head->next = merge_divide(head->next);

    struct list_head *before = head, *after = head->next;
    for (; after != NULL; after = after->next) {
        after->prev = before;
        before = after;
    }
    before->next = head;
    head->prev = before;
}
/* Remove every node which has a node with a strictly less value anywhere to
 * the right side of it */
int q_ascend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    element_t *right = list_entry(head->prev, element_t, list);
    element_t *left = list_entry(head->prev->prev, element_t, list);
    while (&left->list != head) {
        if (strcmp(right->value, left->value) > 0) {
            left = list_entry(left->list.prev, element_t, list);
            right = list_entry(right->list.prev, element_t, list);
        } else {
            list_del(&left->list);
            free(left->value);
            free(left);
            left = list_entry(right->list.prev, element_t, list);
        }
    }
    return q_size(head);
}

/* Remove every node which has a node with a strictly greater value anywhere to
 * the right side of it */
int q_descend(struct list_head *head)
{
    // https://leetcode.com/problems/remove-nodes-from-linked-list/
    if (!head || list_empty(head))
        return 0;

    element_t *right = list_entry(head->prev, element_t, list);
    element_t *left = list_entry(head->prev->prev, element_t, list);
    while (&left->list != head) {
        if (strcmp(right->value, left->value) < 0) {
            left = list_entry(left->list.prev, element_t, list);
            right = list_entry(right->list.prev, element_t, list);
        } else {
            list_del(&left->list);
            free(left->value);
            free(left);
            left = list_entry(right->list.prev, element_t, list);
        }
    }
    return q_size(head);
}

/* Merge all the queues into one sorted queue, which is in ascending/descending
 * order */
int q_merge(struct list_head *head, bool descend)
{
    // https://leetcode.com/problems/merge-k-sorted-lists/
    if (!head || list_empty(head)) {
        return 0;
    }
    if (list_is_singular(head)) {
        return list_entry(head->next, queue_contex_t, chain)->size;
    }
    queue_contex_t *merged_list = list_entry(head->next, queue_contex_t, chain);
    for (struct list_head *p = head->next->next; p != head; p = p->next) {
        queue_contex_t *node = list_entry(p, queue_contex_t, chain);
        list_splice_init(node->q, merged_list->q);
    }
    q_sort(merged_list->q, descend);

    return q_size(merged_list->q);
}
