/* Copy the values to keep into an array and build a new list from them. */
struct ListNode* removeElements(struct ListNode* head, int val) {
    int *keep = malloc(10001 * sizeof *keep), n = 0;
    struct ListNode *out = NULL, *p;

    for (p = head; p; p = p->next)
        if (p->val != val)
            keep[n++] = p->val;
    while (n > 0) {
        p = malloc(sizeof *p);
        p->val = keep[--n];
        p->next = out;
        out = p;
    }
    free(keep);
    return out;
}
