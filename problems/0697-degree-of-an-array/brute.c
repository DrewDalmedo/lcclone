/* Slow and obviously right: work out the degree of nums, then the degree
 * of every stretch nums[l..r], counting each element's copies by comparing
 * it with the rest of the stretch, and keep the shortest stretch that
 * matches. */
static int degree_of(const int *a, int lo, int hi)
{
    int best = 0, i, j, c;

    for (i = lo; i <= hi; i++) {
        c = 0;
        for (j = lo; j <= hi; j++)
            if (a[j] == a[i])
                c++;
        if (c > best)
            best = c;
    }
    return best;
}

int findShortestSubArray(int *nums, int numsSize)
{
    int whole = degree_of(nums, 0, numsSize - 1);
    int best = numsSize, l, r;

    for (l = 0; l < numsSize; l++)
        for (r = l; r < numsSize; r++)
            if (r - l + 1 < best && degree_of(nums, l, r) == whole)
                best = r - l + 1;
    return best;
}
