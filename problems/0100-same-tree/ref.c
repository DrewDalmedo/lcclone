/* Walks both trees in step with a stack of node pairs. A pair that matches is
 * replaced by its two child pairs, so at most 101 pairs wait at once. */
bool isSameTree(struct TreeNode* p, struct TreeNode* q)
{
    struct TreeNode *stack[2 * 101];
    int top = 0;

    stack[top++] = p;
    stack[top++] = q;
    while (top > 0) {
        struct TreeNode *b = stack[--top];
        struct TreeNode *a = stack[--top];

        if (!a && !b)
            continue;
        if (!a || !b || a->val != b->val)
            return false;
        stack[top++] = a->right;
        stack[top++] = b->right;
        stack[top++] = a->left;
        stack[top++] = b->left;
    }
    return true;
}
