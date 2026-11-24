/* One stack and the last node lifted out: the node on top is finished when
 * it has no right child or that child was the last one lifted. */
int* postorderTraversal(struct TreeNode* root, int* returnSize) {
    struct TreeNode *stack[100], *last = NULL;
    int *out = malloc(100 * sizeof *out);
    int top = 0, n = 0;

    while (root || top > 0) {
        struct TreeNode *node;

        while (root) {
            stack[top++] = root;
            root = root->left;
        }
        node = stack[top - 1];
        if (node->right && node->right != last) {
            root = node->right;
        } else {
            out[n++] = node->val;
            last = node;
            top--;
        }
    }
    *returnSize = n;
    return out;
}
