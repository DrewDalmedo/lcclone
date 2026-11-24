/* Depth first: each node adds itself to the tally of its level. */
static void tally(struct TreeNode* p, int level, long long* sum, int* count,
                  int* levels) {
    if (!p)
        return;
    if (level >= *levels)
        *levels = level + 1;
    sum[level] += p->val;
    count[level]++;
    tally(p->left, level + 1, sum, count, levels);
    tally(p->right, level + 1, sum, count, levels);
}

double* averageOfLevels(struct TreeNode* root, int* returnSize) {
    long long* sum = calloc(10001, sizeof *sum);
    int* count = calloc(10001, sizeof *count);
    double* means = malloc(10001 * sizeof *means);
    int levels = 0, i;

    tally(root, 0, sum, count, &levels);
    for (i = 0; i < levels; i++)
        means[i] = (double)sum[i] / count[i];
    free(sum);
    free(count);
    *returnSize = levels;
    return means;
}
