/* Leave the links alone: copy the values out and write them back in
 * reverse order. */
struct ListNode* reverseList(struct ListNode* head) {
    int *v = malloc(5001 * sizeof *v), n = 0;
    struct ListNode *p;

    for (p = head; p; p = p->next)
        v[n++] = p->val;
    for (p = head; p; p = p->next)
        p->val = v[--n];
    free(v);
    return head;
}
