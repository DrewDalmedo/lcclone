/* Breadth first, a level at a time. The sum is kept as a whole number in a
 * long long (at most 4996 * 2^31 in size, so a double holds it exactly) and
 * divided once, which leaves only the rounding of that one division. */
double* averageOfLevels(struct TreeNode* root, int* returnSize) {
    struct TreeNode** queue = malloc(10001 * sizeof *queue);
    double* means = malloc(10001 * sizeof *means);
    int head = 0, tail = 0, levels = 0;

    queue[tail++] = root;
    while (head < tail) {
        int end = tail, count = end - head;
        long long sum = 0;

        for (; head < end; head++) {
            struct TreeNode* p = queue[head];

            sum += p->val;
            if (p->left)
                queue[tail++] = p->left;
            if (p->right)
                queue[tail++] = p->right;
        }
        means[levels++] = (double)sum / count;
    }
    free(queue);
    *returnSize = levels;
    return means;
}
