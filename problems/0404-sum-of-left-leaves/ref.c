/* Visits the nodes with an explicit stack; each node checks whether its own
 * left child is a leaf, since a child can't tell which side it is on. */
int sumOfLeftLeaves(struct TreeNode* root) {
    struct TreeNode** stack = malloc(1002 * sizeof *stack);
    int top = 0, total = 0;

    stack[top++] = root;
    while (top > 0) {
        struct TreeNode* p = stack[--top];
        struct TreeNode* l = p->left;

        if (l && !l->left && !l->right)
            total += l->val;
        else if (l)
            stack[top++] = l;
        if (p->right)
            stack[top++] = p->right;
    }
    free(stack);
    return total;
}
