/* Straight from the definition: left subtree, right subtree, node. */
static void clear_out(struct TreeNode *node, int *out, int *n) {
    if (!node)
        return;
    clear_out(node->left, out, n);
    clear_out(node->right, out, n);
    out[(*n)++] = node->val;
}

int* postorderTraversal(struct TreeNode* root, int* returnSize) {
    int *out = malloc(100 * sizeof *out);

    *returnSize = 0;
    clear_out(root, out, returnSize);
    return out;
}
