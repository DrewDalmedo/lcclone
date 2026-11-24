/* All values are equal exactly when the smallest is the largest. */
static void scan(struct TreeNode *t, int *lo, int *hi)
{
    if (!t)
        return;
    if (t->val < *lo)
        *lo = t->val;
    if (t->val > *hi)
        *hi = t->val;
    scan(t->left, lo, hi);
    scan(t->right, lo, hi);
}

bool isUnivalTree(struct TreeNode* root)
{
    int lo = root->val, hi = root->val;

    scan(root, &lo, &hi);
    return lo == hi;
}
