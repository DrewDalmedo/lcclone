/* Swap the children of each node, with a stack of nodes still to swap. */
struct TreeNode* invertTree(struct TreeNode* root) {
    struct TreeNode *stack[100];
    int top = 0;

    if (root)
        stack[top++] = root;
    while (top > 0) {
        struct TreeNode *node = stack[--top];
        struct TreeNode *old_left = node->left;

        node->left = node->right;
        node->right = old_left;
        if (node->left)
            stack[top++] = node->left;
        if (node->right)
            stack[top++] = node->right;
    }
    return root;
}
