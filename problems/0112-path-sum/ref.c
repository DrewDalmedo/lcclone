/* Depth first with an explicit stack of (node, score including that node).
 * Each pop adds at most one net entry per level, so the stack stays within
 * the node count. */
bool hasPathSum(struct TreeNode* root, int targetSum) {
    struct item {
        struct TreeNode* node;
        int score;
    } *stack;
    int top = 0;
    bool found = false;

    if (!root)
        return false;
    stack = malloc(5002 * sizeof *stack);
    stack[top].node = root;
    stack[top++].score = root->val;
    while (top > 0 && !found) {
        struct item it = stack[--top];
        struct TreeNode* p = it.node;

        if (!p->left && !p->right) {
            found = it.score == targetSum;
            continue;
        }
        if (p->right) {
            stack[top].node = p->right;
            stack[top++].score = it.score + p->right->val;
        }
        if (p->left) {
            stack[top].node = p->left;
            stack[top++].score = it.score + p->left->val;
        }
    }
    free(stack);
    return found;
}
