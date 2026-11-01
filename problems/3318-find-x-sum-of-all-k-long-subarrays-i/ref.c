/* Reference solution: slide the window along nums, keeping how many times
 * each value 1..50 appears in it. For each window, list the values that
 * appear, sort them by count (larger first, then by value, larger first)
 * and add count * value over the first x of them. */
static int count[51];

static int by_rank(const void *pa, const void *pb)
{
    int a = *(const int *)pa, b = *(const int *)pb;

    if (count[a] != count[b])
        return count[b] - count[a];
    return b - a;
}

int *findXSum(int *nums, int numsSize, int k, int x, int *returnSize)
{
    int windows = numsSize - k + 1;
    int *ans = malloc((size_t)windows * sizeof *ans);
    int vals[50];
    int i, v, m, sum;

    memset(count, 0, sizeof count);
    for (i = 0; i < k; i++)
        count[nums[i]]++;
    for (i = 0; i < windows; i++) {
        if (i > 0) {
            count[nums[i - 1]]--;
            count[nums[i + k - 1]]++;
        }
        m = 0;
        for (v = 1; v <= 50; v++)
            if (count[v] > 0)
                vals[m++] = v;
        qsort(vals, (size_t)m, sizeof *vals, by_rank);
        sum = 0;
        for (v = 0; v < m && v < x; v++)
            sum += count[vals[v]] * vals[v];
        ans[i] = sum;
    }
    *returnSize = windows;
    return ans;
}
