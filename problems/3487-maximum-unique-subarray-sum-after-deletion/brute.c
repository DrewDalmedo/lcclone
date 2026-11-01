/* Slow and obviously right: play the two moves every way. Try every set
 * of survivors (at least one), then every block of them, and keep the best
 * sum of a block whose values are all different. Only for small cases:
 * at most 15 entries. */
int maxSum(int *nums, int numsSize)
{
    int keep[16], best = 0, found = 0, mask, m, i, j, a, b, sum, ok;

    for (mask = 1; mask < 1 << numsSize; mask++) {
        m = 0;
        for (i = 0; i < numsSize; i++)
            if (mask >> i & 1)
                keep[m++] = nums[i];
        for (a = 0; a < m; a++)
            for (b = a; b < m; b++) {
                ok = 1;
                sum = 0;
                for (i = a; i <= b; i++) {
                    sum += keep[i];
                    for (j = a; j < i; j++)
                        if (keep[j] == keep[i])
                            ok = 0;
                }
                if (ok && (!found || sum > best)) {
                    best = sum;
                    found = 1;
                }
            }
    }
    return best;
}
