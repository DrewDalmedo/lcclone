/* Iterative, with a stack of nodes still to do. */
struct TreeNode* invertTree(struct TreeNode* root) {
    struct TreeNode** todo;
    int n = 0, cap = 64;

    if (!root)
        return root;
    todo = malloc(cap * sizeof *todo);
    todo[n++] = root;
    while (n > 0) {
        struct TreeNode* p = todo[--n];
        struct TreeNode* t = p->left;

        p->left = p->right;
        p->right = t;
        if (n + 2 > cap) {
            cap *= 2;
            todo = realloc(todo, cap * sizeof *todo);
        }
        if (p->left)
            todo[n++] = p->left;
        if (p->right)
            todo[n++] = p->right;
    }
    free(todo);
    return root;
}
