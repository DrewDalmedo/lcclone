/* From the definition: gather the values by recursion and try every pair of
 * different nodes. */
static int tree_size(struct TreeNode* node) {
    return node ? 1 + tree_size(node->left) + tree_size(node->right) : 0;
}

static void gather(struct TreeNode* node, int* out, int* n) {
    if (!node)
        return;
    gather(node->left, out, n);
    out[(*n)++] = node->val;
    gather(node->right, out, n);
}

bool findTarget(struct TreeNode* root, int k) {
    int total = tree_size(root), n = 0, i, j;
    int* vals = malloc((size_t)total * sizeof *vals);
    bool found = false;

    gather(root, vals, &n);
    for (i = 0; i < n && !found; i++)
        for (j = i + 1; j < n; j++)
            if (vals[i] + vals[j] == k) {
                found = true;
                break;
            }
    free(vals);
    return found;
}
