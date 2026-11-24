/* Depth first with a stack that grows as needed. A side is entered only when
 * the node's value leaves room for it to hold something in the band. */
int rangeSumBST(struct TreeNode* root, int low, int high) {
    size_t cap = 64, top = 0;
    struct TreeNode **stack = malloc(cap * sizeof *stack);
    int sum = 0;

    stack[top++] = root;
    while (top > 0) {
        struct TreeNode *t = stack[--top];

        if (top + 2 > cap) {
            cap *= 2;
            stack = realloc(stack, cap * sizeof *stack);
        }
        if (low <= t->val && t->val <= high)
            sum += t->val;
        if (t->left && t->val > low)
            stack[top++] = t->left;
        if (t->right && t->val < high)
            stack[top++] = t->right;
    }
    free(stack);
    return sum;
}
