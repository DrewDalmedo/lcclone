/* The station just before or after any other on the line is among its
 * ancestors or descendants, so it is enough to compare each node with its
 * nearest ancestor on either side, carried down on an explicit stack. */
struct gap_frame {
    struct TreeNode* node;
    int lo, hi;                 /* nearest ancestors around it, -1 if none */
};

int minDiffInBST(struct TreeNode* root) {
    size_t cap = 128, top = 0;
    struct gap_frame* stack = malloc(cap * sizeof *stack);
    int best = INT_MAX;

    stack[top].node = root;
    stack[top].lo = -1;
    stack[top].hi = -1;
    top++;
    while (top > 0) {
        struct gap_frame f = stack[--top];
        int v = f.node->val;

        if (f.lo >= 0 && v - f.lo < best)
            best = v - f.lo;
        if (f.hi >= 0 && f.hi - v < best)
            best = f.hi - v;
        if (top + 2 > cap) {
            cap *= 2;
            stack = realloc(stack, cap * sizeof *stack);
        }
        if (f.node->left) {
            stack[top].node = f.node->left;
            stack[top].lo = f.lo;
            stack[top].hi = v;
            top++;
        }
        if (f.node->right) {
            stack[top].node = f.node->right;
            stack[top].lo = v;
            stack[top].hi = f.hi;
            top++;
        }
    }
    free(stack);
    return best;
}
