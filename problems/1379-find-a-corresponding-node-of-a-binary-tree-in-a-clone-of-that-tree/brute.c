/* The values are distinct, so search cloned level by level for target's. */
struct TreeNode* getTargetCopy(struct TreeNode* original, struct TreeNode* cloned,
                               struct TreeNode* target) {
    struct TreeNode **queue = malloc(10001 * sizeof *queue), *found = NULL;
    int head = 0, tail = 0;

    (void)original;
    queue[tail++] = cloned;
    while (head < tail) {
        struct TreeNode *p = queue[head++];

        if (p->val == target->val) {
            found = p;
            break;
        }
        if (p->left)
            queue[tail++] = p->left;
        if (p->right)
            queue[tail++] = p->right;
    }
    free(queue);
    return found;
}
