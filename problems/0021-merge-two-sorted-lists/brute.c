/* Put every node in one array, insertion-sort it by value, relink. */
struct ListNode* mergeTwoLists(struct ListNode* list1, struct ListNode* list2) {
    struct ListNode *all[100], *p;
    int n = 0, i, j;

    for (p = list1; p; p = p->next)
        all[n++] = p;
    for (p = list2; p; p = p->next)
        all[n++] = p;
    for (i = 1; i < n; i++) {
        p = all[i];
        for (j = i; j > 0 && all[j - 1]->val > p->val; j--)
            all[j] = all[j - 1];
        all[j] = p;
    }
    for (i = 0; i + 1 < n; i++)
        all[i]->next = all[i + 1];
    if (n == 0)
        return NULL;
    all[n - 1]->next = NULL;
    return all[0];
}
