/* Compares every node with the root, using a stack of nodes to visit. */
bool isUnivalTree(struct TreeNode* root)
{
    struct TreeNode *stack[101];
    int top = 0;

    stack[top++] = root;
    while (top > 0) {
        struct TreeNode *x = stack[--top];

        if (x->val != root->val)
            return false;
        if (x->right)
            stack[top++] = x->right;
        if (x->left)
            stack[top++] = x->left;
    }
    return true;
}
