/* Iterative, with a stack of nodes still to do. */
struct Node* mirror(struct Node* root) {
    struct Node** todo;
    int n = 0, cap = 64;

    if (!root)
        return root;
    todo = malloc(cap * sizeof *todo);
    todo[n++] = root;
    while (n > 0) {
        struct Node* p = todo[--n];
        int i;

        for (i = 0; i < p->numChildren / 2; i++) {
            struct Node* t = p->children[i];

            p->children[i] = p->children[p->numChildren - 1 - i];
            p->children[p->numChildren - 1 - i] = t;
        }
        for (i = 0; i < p->numChildren; i++) {
            if (n == cap) {
                cap *= 2;
                todo = realloc(todo, cap * sizeof *todo);
            }
            todo[n++] = p->children[i];
        }
    }
    free(todo);
    return root;
}
