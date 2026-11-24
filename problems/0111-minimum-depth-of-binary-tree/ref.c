/* Breadth first, a level at a time: the first leaf met is on the lowest
 * level number there is. No recursion. */
int minDepth(struct TreeNode* root) {
    struct TreeNode** queue;
    int head = 0, tail = 0, level = 0;

    if (!root)
        return 0;
    queue = malloc(100001 * sizeof *queue);
    queue[tail++] = root;
    for (;;) {
        int end = tail;

        level++;
        for (; head < end; head++) {
            struct TreeNode* p = queue[head];

            if (!p->left && !p->right) {
                free(queue);
                return level;
            }
            if (p->left)
                queue[tail++] = p->left;
            if (p->right)
                queue[tail++] = p->right;
        }
    }
}
