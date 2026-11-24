/* An in-order walk with an explicit stack brings equal values together, so
 * one pass over the runs finds the highest count and who reaches it. */
int* findMode(struct TreeNode* root, int* returnSize) {
    size_t cap = 128, top = 0, rescap = 16, nres = 0;
    struct TreeNode** stack = malloc(cap * sizeof *stack);
    int* res = malloc(rescap * sizeof *res);
    struct TreeNode* cur = root;
    int best = 0, run = 0, prev = 0, seen = 0;

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
        if (!seen || cur->val != prev) {
            seen = 1;
            prev = cur->val;
            run = 0;
        }
        run++;
        if (run > best) {
            best = run;
            nres = 0;
        }
        if (run == best) {
            if (nres == rescap) {
                rescap *= 2;
                res = realloc(res, rescap * sizeof *res);
            }
            res[nres++] = cur->val;
        }
        cur = cur->right;
    }
    free(stack);
    *returnSize = (int)nres;
    return res;
}
