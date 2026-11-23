/* The definition, recursively. */
static void visit(struct Node* p, int* out, int* n) {
    int i;

    if (!p)
        return;
    for (i = 0; i < p->numChildren; i++)
        visit(p->children[i], out, n);
    out[(*n)++] = p->val;
}

int* postorder(struct Node* root, int* returnSize) {
    int *out = malloc(10001 * sizeof *out);

    *returnSize = 0;
    visit(root, out, returnSize);
    return out;
}
