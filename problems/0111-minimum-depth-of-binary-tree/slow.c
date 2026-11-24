/* Right but O(n * depth): list the nodes level by level, each with the
 * place of its parent, then from every leaf count the steps back up to the
 * root. On a caterpillar that is 5 * 10^4 walks of up to 5 * 10^4 steps. */
int minDepth(struct TreeNode* root) {
    struct TreeNode **q;
    int *up, head, tail = 1, best = INT_MAX;

    if (!root)
        return 0;
    q = malloc(100000 * sizeof *q);
    up = malloc(100000 * sizeof *up);
    q[0] = root;
    up[0] = -1;
    for (head = 0; head < tail; head++) {
        struct TreeNode *p = q[head];

        if (p->left) {
            q[tail] = p->left;
            up[tail++] = head;
        }
        if (p->right) {
            q[tail] = p->right;
            up[tail++] = head;
        }
        if (!p->left && !p->right) {
            int d = 0, x;

            for (x = head; x >= 0; x = up[x])
                d++;
            if (d < best)
                best = d;
        }
    }
    free(q);
    free(up);
    return best;
}
