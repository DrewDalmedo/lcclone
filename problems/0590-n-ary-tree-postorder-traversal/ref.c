/* Visit node, then children last to first, and reverse the result: that is
 * children first to last, then node. */
int* postorder(struct Node* root, int* returnSize) {
    struct Node **stack = malloc(10001 * sizeof *stack);
    int *out = malloc(10001 * sizeof *out);
    int top = 0, n = 0, i;

    if (root)
        stack[top++] = root;
    while (top > 0) {
        struct Node *p = stack[--top];

        out[n++] = p->val;
        for (i = 0; i < p->numChildren; i++)
            stack[top++] = p->children[i];
    }
    for (i = 0; i < n / 2; i++) {
        int t = out[i];

        out[i] = out[n - 1 - i];
        out[n - 1 - i] = t;
    }
    free(stack);
    *returnSize = n;
    return out;
}
