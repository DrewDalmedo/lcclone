/* Read the values in order by recursion, then build a new chain of nodes
 * from the largest up. */
static int count_nodes(struct TreeNode *t) {
    return t ? 1 + count_nodes(t->left) + count_nodes(t->right) : 0;
}

static void read_inorder(struct TreeNode *t, int *out, int *k) {
    if (!t)
        return;
    read_inorder(t->left, out, k);
    out[(*k)++] = t->val;
    read_inorder(t->right, out, k);
}

struct TreeNode* increasingBST(struct TreeNode* root) {
    int n = count_nodes(root), k = 0, i;
    int *vals = malloc((size_t)n * sizeof *vals);
    struct TreeNode *head = NULL;

    read_inorder(root, vals, &k);
    for (i = n - 1; i >= 0; i--) {
        struct TreeNode *node = malloc(sizeof *node);

        node->val = vals[i];
        node->left = NULL;
        node->right = head;
        head = node;
    }
    free(vals);
    return head;
}
