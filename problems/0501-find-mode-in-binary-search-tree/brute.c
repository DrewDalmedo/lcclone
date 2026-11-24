/* Straight from the definition: count every value in a table, by recursion,
 * then keep the values with the highest count. */
static void tally(struct TreeNode* node, int* counts) {
    if (!node)
        return;
    counts[node->val + 100000]++;
    tally(node->left, counts);
    tally(node->right, counts);
}

int* findMode(struct TreeNode* root, int* returnSize) {
    int* counts = calloc(200001, sizeof *counts);
    int* out;
    int best = 0, m = 0, i;

    tally(root, counts);
    for (i = 0; i <= 200000; i++)
        if (counts[i] > best)
            best = counts[i];
    for (i = 0; i <= 200000; i++)
        if (counts[i] == best)
            m++;
    out = malloc((size_t)m * sizeof *out);
    m = 0;
    for (i = 0; i <= 200000; i++)
        if (counts[i] == best)
            out[m++] = i - 100000;
    free(counts);
    *returnSize = m;
    return out;
}
