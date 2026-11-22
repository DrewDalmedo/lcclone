struct TreeNode** chains(struct ListNode** lists, int listsSize,
                         int* returnSize) {
    struct TreeNode** out = malloc((listsSize + 1) * sizeof *out);
    int i;

    for (i = 0; i < listsSize; i++) {
        struct TreeNode** at = &out[i];
        struct ListNode* p;

        *at = NULL;
        for (p = lists[i]; p; p = p->next) {
            *at = calloc(1, sizeof **at);
            (*at)->val = p->val;
            at = &(*at)->right;
        }
    }
    *returnSize = listsSize;
    return out;
}
