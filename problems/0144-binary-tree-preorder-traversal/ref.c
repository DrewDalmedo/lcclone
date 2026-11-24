/* A stack of nodes still to visit. The right child goes on first, so that
 * the left one comes off first. */
int* preorderTraversal(struct TreeNode* root, int* returnSize) {
    struct TreeNode *stack[100];
    int *out = malloc(100 * sizeof *out);
    int top = 0, n = 0;

    if (root)
        stack[top++] = root;
    while (top > 0) {
        struct TreeNode *node = stack[--top];

        out[n++] = node->val;
        if (node->right)
            stack[top++] = node->right;
        if (node->left)
            stack[top++] = node->left;
    }
    *returnSize = n;
    return out;
}
