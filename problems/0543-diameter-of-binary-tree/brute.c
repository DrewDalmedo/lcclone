/* Recursively: the longest route turning at a node goes down both of its
 * sides, and the answer is the best over all nodes. */
static int reach(struct TreeNode* p, int* best) {
    int l, r;

    if (!p)
        return 0;
    l = reach(p->left, best);
    r = reach(p->right, best);
    if (l + r > *best)
        *best = l + r;
    return 1 + (l > r ? l : r);
}

int diameterOfBinaryTree(struct TreeNode* root) {
    int best = 0;

    reach(root, &best);
    return best;
}
