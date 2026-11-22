/* Breadth first, a level at a time: no recursion, so the deepest trees
 * need no stack. */
int maxDepth(struct TreeNode* root) {
    struct TreeNode** queue;
    int head = 0, tail = 0, levels = 0;

    if (!root)
        return 0;
    queue = malloc(10001 * sizeof *queue);
    queue[tail++] = root;
    while (head < tail) {
        int end = tail;

        levels++;
        for (; head < end; head++) {
            if (queue[head]->left)
                queue[tail++] = queue[head]->left;
            if (queue[head]->right)
                queue[tail++] = queue[head]->right;
        }
    }
    free(queue);
    return levels;
}
