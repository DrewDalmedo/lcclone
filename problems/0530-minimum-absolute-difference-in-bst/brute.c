/* From the definition: gather the values by recursion and try every pair. */
static int count_nodes(struct TreeNode* node) {
    return node ? 1 + count_nodes(node->left) + count_nodes(node->right) : 0;
}

static void gather(struct TreeNode* node, int* out, int* n) {
    if (!node)
        return;
    out[(*n)++] = node->val;
    gather(node->left, out, n);
    gather(node->right, out, n);
}

int getMinimumDifference(struct TreeNode* root) {
    int total = count_nodes(root), n = 0, best = INT_MAX, i, j;
    int* vals = malloc((size_t)total * sizeof *vals);

    gather(root, vals, &n);
    for (i = 0; i < n; i++)
        for (j = i + 1; j < n; j++) {
            int d = abs(vals[i] - vals[j]);

            if (d < best)
                best = d;
        }
    free(vals);
    return best;
}
