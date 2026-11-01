/* Slow and obviously right, with other bookkeeping: the array at any
 * moment splits nums into runs of neighbours, and each of its elements is
 * the sum of one run, added up again from nums whenever it is needed. The
 * runs are kept as the index where each one starts. A fusion joins two
 * neighbouring runs, so it forgets where the right one starts. */
static int run_sum(const int *nums, int n, const int *start, int runs, int k)
{
    int end = k + 1 < runs ? start[k + 1] : n, sum = 0, i;

    for (i = start[k]; i < end; i++)
        sum += nums[i];
    return sum;
}

int minimumPairRemoval(int *nums, int numsSize)
{
    int start[50], runs = numsSize, fusions = 0, k, best, best_sum, sum;
    int sorted;

    for (k = 0; k < runs; k++)
        start[k] = k;
    for (;;) {
        sorted = 1;
        for (k = 0; k + 1 < runs; k++)
            if (run_sum(nums, numsSize, start, runs, k) >
                run_sum(nums, numsSize, start, runs, k + 1))
                sorted = 0;
        if (sorted)
            return fusions;
        best = -1;
        best_sum = 0;
        for (k = 0; k + 1 < runs; k++) {
            sum = run_sum(nums, numsSize, start, runs, k) +
                  run_sum(nums, numsSize, start, runs, k + 1);
            if (best < 0 || sum < best_sum || (sum == best_sum && k < best)) {
                best = k;
                best_sum = sum;
            }
        }
        for (k = best + 1; k + 1 < runs; k++)
            start[k] = start[k + 1];
        runs--;
        fusions++;
    }
}
