/* Collect every value by recursion, sort, and take the first above the
 * smallest. */
static void gather(struct TreeNode *t, int *out, int *count) {
    if (!t)
        return;
    out[(*count)++] = t->val;
    gather(t->left, out, count);
    gather(t->right, out, count);
}

int findSecondMinimumValue(struct TreeNode* root) {
    int vals[32], count = 0, i, j, x;

    gather(root, vals, &count);
    for (i = 1; i < count; i++) {
        x = vals[i];
        for (j = i; j > 0 && vals[j - 1] > x; j--)
            vals[j] = vals[j - 1];
        vals[j] = x;
    }
    for (i = 1; i < count; i++)
        if (vals[i] > vals[0])
            return vals[i];
    return -1;
}
