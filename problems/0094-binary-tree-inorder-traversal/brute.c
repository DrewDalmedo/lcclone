/* Straight from the definition: left subtree, node, right subtree. */
static void lay_out(struct TreeNode *node, int *out, int *n) {
    if (!node)
        return;
    lay_out(node->left, out, n);
    out[(*n)++] = node->val;
    lay_out(node->right, out, n);
}

int* inorderTraversal(struct TreeNode* root, int* returnSize) {
    int *out = malloc(100 * sizeof *out);

    *returnSize = 0;
    lay_out(root, out, returnSize);
    return out;
}
