/* Recursion that keeps the route as numbers and writes each leaf's text from
 * scratch when it gets there. */
static void walk(struct TreeNode* p, int* route, int depth, char** out, int* count) {
    route[depth++] = p->val;
    if (!p->left && !p->right) {
        char* s = malloc(6 * (size_t)depth + 1);
        int len = 0, i;

        for (i = 0; i < depth; i++)
            len += sprintf(s + len, i ? "->%d" : "%d", route[i]);
        out[(*count)++] = s;
        return;
    }
    if (p->left)
        walk(p->left, route, depth, out, count);
    if (p->right)
        walk(p->right, route, depth, out, count);
}

char** binaryTreePaths(struct TreeNode* root, int* returnSize) {
    int route[101];
    char** out = malloc(64 * sizeof *out);

    *returnSize = 0;
    walk(root, route, 0, out, returnSize);
    return out;
}
