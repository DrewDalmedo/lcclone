/* An explicit stack: children go on in reverse so the first comes off next. */
int* preorder(struct Node* root, int* returnSize) {
    struct Node **stack = malloc(10001 * sizeof *stack);
    int *out = malloc(10001 * sizeof *out);
    int top = 0, n = 0, i;

    if (root)
        stack[top++] = root;
    while (top > 0) {
        struct Node *p = stack[--top];

        out[n++] = p->val;
        for (i = p->numChildren - 1; i >= 0; i--)
            stack[top++] = p->children[i];
    }
    free(stack);
    *returnSize = n;
    return out;
}
