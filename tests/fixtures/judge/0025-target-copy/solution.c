/* Walks both trees side by side, with a stack. */
struct TreeNode* getTargetCopy(struct TreeNode* original,
                               struct TreeNode* cloned,
                               struct TreeNode* target) {
    struct TreeNode* stack[2 * 64];
    int n = 0;

    stack[n++] = original;
    stack[n++] = cloned;
    while (n > 0) {
        struct TreeNode* c = stack[--n];
        struct TreeNode* o = stack[--n];

        if (!o)
            continue;
        if (o == target)
            return c;
        stack[n++] = o->left;
        stack[n++] = c->left;
        stack[n++] = o->right;
        stack[n++] = c->right;
    }
    return NULL;
}
