/* Every route has one highest node, where it turns. Lists the nodes breadth
 * first, then goes through them backwards, so that each node knows how far
 * down each of its sides reaches. No recursion. */
int diameterOfBinaryTree(struct TreeNode* root) {
    struct item {
        struct TreeNode* node;
        int parent, left, right;
    } *q = malloc(10001 * sizeof *q);
    int head, tail = 1, best = 0;

    q[0].node = root;
    q[0].parent = -1;
    q[0].left = q[0].right = 0;
    for (head = 0; head < tail; head++) {
        struct TreeNode* kids[2];
        int k;

        kids[0] = q[head].node->left;
        kids[1] = q[head].node->right;
        for (k = 0; k < 2; k++) {
            if (kids[k]) {
                q[tail].node = kids[k];
                q[tail].parent = head;
                q[tail].left = q[tail].right = 0;
                tail++;
            }
        }
    }
    for (head = tail - 1; head >= 0; head--) {
        int l = q[head].left, r = q[head].right, p = q[head].parent;
        int reach = 1 + (l > r ? l : r);

        if (l + r > best)
            best = l + r;
        if (p >= 0 && q[p].node->left == q[head].node)
            q[p].left = reach;
        else if (p >= 0)
            q[p].right = reach;
    }
    free(q);
    return best;
}
