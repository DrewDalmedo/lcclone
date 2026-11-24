/* An in-order walk visits the values in increasing order, and the closest
 * two values are next to each other in it. The stack is explicit, so a path
 * of 10^4 nodes costs no recursion. */
int getMinimumDifference(struct TreeNode* root) {
    size_t cap = 128, top = 0;
    struct TreeNode** stack = malloc(cap * sizeof *stack);
    struct TreeNode* cur = root;
    int best = INT_MAX, prev = 0, seen = 0;

    while (cur || top > 0) {
        while (cur) {
            if (top == cap) {
                cap *= 2;
                stack = realloc(stack, cap * sizeof *stack);
            }
            stack[top++] = cur;
            cur = cur->left;
        }
        cur = stack[--top];
        if (seen && cur->val - prev < best)
            best = cur->val - prev;
        prev = cur->val;
        seen = 1;
        cur = cur->right;
    }
    free(stack);
    return best;
}
