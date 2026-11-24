/* Depth first with an explicit stack of (node, number read so far). Doubling
 * and adding the bit never overflows: a prefix of a route is at most that
 * route's number, which the guarantee keeps within an int. */
int sumRootToLeaf(struct TreeNode* root) {
    struct item {
        struct TreeNode* node;
        int value;
    } *stack = malloc(1002 * sizeof *stack);
    int top = 0, total = 0;

    stack[top].node = root;
    stack[top++].value = root->val;
    while (top > 0) {
        struct item it = stack[--top];
        struct TreeNode* p = it.node;

        if (!p->left && !p->right) {
            total += it.value;
            continue;
        }
        if (p->right) {
            stack[top].node = p->right;
            stack[top++].value = 2 * it.value + p->right->val;
        }
        if (p->left) {
            stack[top].node = p->left;
            stack[top++].value = 2 * it.value + p->left->val;
        }
    }
    free(stack);
    return total;
}
