/* Note which values occur, then build a new list of them, largest first
 * so that each new node goes in front. */
struct ListNode* deleteDuplicates(struct ListNode* head) {
    char seen[201] = {0};
    struct ListNode *out = NULL, *p;
    int v;

    for (p = head; p; p = p->next)
        seen[p->val + 100] = 1;
    for (v = 100; v >= -100; v--) {
        if (seen[v + 100]) {
            p = malloc(sizeof *p);
            p->val = v;
            p->next = out;
            out = p;
        }
    }
    return out;
}
