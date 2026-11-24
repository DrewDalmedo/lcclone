/* Measure every pair of stations: each node against all the nodes visited
 * before it, in a recursive walk. */
static void visit(struct TreeNode* node, int* seen, int* n, int* best) {
    int i;

    if (!node)
        return;
    for (i = 0; i < *n; i++) {
        int d = seen[i] > node->val ? seen[i] - node->val : node->val - seen[i];

        if (d < *best)
            *best = d;
    }
    seen[(*n)++] = node->val;
    visit(node->left, seen, n, best);
    visit(node->right, seen, n, best);
}

int minDiffInBST(struct TreeNode* root) {
    int seen[100], n = 0, best = INT_MAX;

    visit(root, seen, &n, &best);
    return best;
}
