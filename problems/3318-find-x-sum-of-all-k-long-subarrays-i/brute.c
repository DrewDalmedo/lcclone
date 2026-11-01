/* Slow and obviously right, straight from the definition: for each window
 * and each element of it, count how many distinct values of the window
 * rank above the element's value (appear more often, or as often and are
 * larger). The element counts toward the x-sum when fewer than x do. */
static int occurrences(const int *w, int k, int v)
{
    int c = 0, j;

    for (j = 0; j < k; j++)
        if (w[j] == v)
            c++;
    return c;
}

int *findXSum(int *nums, int numsSize, int k, int x, int *returnSize)
{
    int windows = numsSize - k + 1;
    int *ans = malloc((size_t)windows * sizeof *ans);
    int i, j, u, t, above, first, cv, cu;
    const int *w;

    for (i = 0; i < windows; i++) {
        w = nums + i;
        ans[i] = 0;
        for (j = 0; j < k; j++) {
            cv = occurrences(w, k, w[j]);
            above = 0;
            for (u = 0; u < k; u++) {
                /* look at each distinct value once: at its first index */
                first = 1;
                for (t = 0; t < u; t++)
                    if (w[t] == w[u])
                        first = 0;
                if (!first || w[u] == w[j])
                    continue;
                cu = occurrences(w, k, w[u]);
                if (cu > cv || (cu == cv && w[u] > w[j]))
                    above++;
            }
            if (above < x)
                ans[i] += w[j];
        }
    }
    *returnSize = windows;
    return ans;
}
