/* Breadth first: one level at a time, with a queue. */
int depth(struct TreeNode* root) {
    struct TreeNode** q;
    int head = 0, tail = 0, cap = 1024, levels = 0;

    if (!root)
        return 0;
    q = malloc(cap * sizeof *q);
    q[tail++] = root;
    while (head < tail) {
        int end = tail;

        levels++;
        for (; head < end; head++) {
            struct TreeNode* kid[2] = {q[head]->left, q[head]->right};
            int k;

            for (k = 0; k < 2; k++) {
                if (!kid[k])
                    continue;
                if (tail == cap) {
                    cap *= 2;
                    q = realloc(q, cap * sizeof *q);
                }
                q[tail++] = kid[k];
            }
        }
    }
    free(q);
    return levels;
}
