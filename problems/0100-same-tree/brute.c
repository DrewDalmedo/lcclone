/* Writes each tree out in preorder with a marker for every missing child,
 * which fixes the tree exactly, and compares the two lists. */
static int flatten(struct TreeNode *t, int *out, int k)
{
    if (!t) {
        out[k++] = 1000000; /* outside the range of values */
        return k;
    }
    out[k++] = t->val;
    k = flatten(t->left, out, k);
    return flatten(t->right, out, k);
}

bool isSameTree(struct TreeNode* p, struct TreeNode* q)
{
    int a[201], b[201]; /* n nodes make 2n + 1 items */
    int na = flatten(p, a, 0), nb = flatten(q, b, 0), i;

    if (na != nb)
        return false;
    for (i = 0; i < na; i++)
        if (a[i] != b[i])
            return false;
    return true;
}
