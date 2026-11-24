/* Straight from the definition: node, left subtree, right subtree. */
static void note_down(struct TreeNode *node, int *out, int *n) {
    if (!node)
        return;
    out[(*n)++] = node->val;
    note_down(node->left, out, n);
    note_down(node->right, out, n);
}

int* preorderTraversal(struct TreeNode* root, int* returnSize) {
    int *out = malloc(100 * sizeof *out);

    *returnSize = 0;
    note_down(root, out, returnSize);
    return out;
}
