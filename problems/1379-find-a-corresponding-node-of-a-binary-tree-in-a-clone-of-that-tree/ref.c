/* Walk both trees in step with one explicit stack of pairs, and compare
 * nodes of original with target by address. */
struct TreeNode* getTargetCopy(struct TreeNode* original, struct TreeNode* cloned,
                               struct TreeNode* target) {
    struct TreeNode **stack = malloc(2 * 10001 * sizeof *stack);
    struct TreeNode *found = NULL;
    int top = 0;

    stack[top++] = original;
    stack[top++] = cloned;
    while (top > 0) {
        struct TreeNode *c = stack[--top], *o = stack[--top];

        if (o == target) {
            found = c;
            break;
        }
        if (o->right) {
            stack[top++] = o->right;
            stack[top++] = c->right;
        }
        if (o->left) {
            stack[top++] = o->left;
            stack[top++] = c->left;
        }
    }
    free(stack);
    return found;
}
