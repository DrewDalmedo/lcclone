/* Writes a tree in preorder as numbers, with a marker for each missing child.
 * That form is self-delimiting, so a branch of root equals subRoot exactly when
 * subRoot's list occurs in root's list. KMP finds it in O(n + m). */
static int flatten(struct TreeNode *t, int *out)
{
    struct TreeNode *stack[2002];
    int top = 0, k = 0;

    stack[top++] = t;
    while (top > 0) {
        struct TreeNode *x = stack[--top];

        if (!x) {
            out[k++] = 1000000; /* outside the range of values */
            continue;
        }
        out[k++] = x->val;
        stack[top++] = x->right;
        stack[top++] = x->left;
    }
    return k;
}

bool isSubtree(struct TreeNode* root, struct TreeNode* subRoot)
{
    int text[4001], pat[2001], fail[2001];
    int n = flatten(root, text), m = flatten(subRoot, pat), i, k;

    fail[0] = 0;
    for (i = 1, k = 0; i < m; i++) {
        while (k > 0 && pat[i] != pat[k])
            k = fail[k - 1];
        if (pat[i] == pat[k])
            k++;
        fail[i] = k;
    }
    for (i = 0, k = 0; i < n; i++) {
        while (k > 0 && text[i] != pat[k])
            k = fail[k - 1];
        if (text[i] == pat[k])
            k++;
        if (k == m)
            return true;
    }
    return false;
}
