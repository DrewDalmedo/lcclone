/* Collects each tree's whole fringe, then compares the two lists. */
static void collect(struct TreeNode *t, int *out, int *k)
{
    if (!t)
        return;
    if (!t->left && !t->right) {
        out[(*k)++] = t->val;
        return;
    }
    collect(t->left, out, k);
    collect(t->right, out, k);
}

bool leafSimilar(struct TreeNode* root1, struct TreeNode* root2)
{
    int a[101], b[101], na = 0, nb = 0, i;

    collect(root1, a, &na);
    collect(root2, b, &nb);
    if (na != nb)
        return false;
    for (i = 0; i < na; i++)
        if (a[i] != b[i])
            return false;
    return true;
}
