/* Reference solution: only the smallest element m and the largest M
 * matter. Raising m by k and lowering M by k narrows the gap by 2k, and
 * every other element lies between them and can follow either end; once
 * the gap is 2k or less, all elements can meet at one value. So the answer
 * is max(0, M - m - 2k). */
int smallestRangeI(int *nums, int numsSize, int k)
{
    int lo = nums[0], hi = nums[0], i, spread;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] < lo)
            lo = nums[i];
        if (nums[i] > hi)
            hi = nums[i];
    }
    spread = hi - lo - 2 * k;
    return spread > 0 ? spread : 0;
}
