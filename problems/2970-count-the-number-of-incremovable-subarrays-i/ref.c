/* Reference solution: two pointers, O(n). Let nums[0..p] be the longest
 * strictly rising prefix; if it is all of nums, each of the n(n+1)/2
 * stretches works. Otherwise count the stretches by j, the first position
 * kept after the cut (j = n when nothing after it is kept). j must lie in
 * the strictly rising suffix, and the part kept before the cut is
 * nums[0..i - 1] for some i from 0 to k + 1, where nums[0..k] is the
 * longest piece of the rising prefix whose values are all below nums[j].
 * As j moves left, nums[j] falls, so k only ever moves left too. */
int incremovableSubarrayCount(int *nums, int numsSize)
{
    int n = numsSize, p = 0, k, j, count;

    while (p + 1 < n && nums[p] < nums[p + 1])
        p++;
    if (p == n - 1)
        return n * (n + 1) / 2;
    count = p + 2;              /* j = n: keep 0 to p + 1 leading elements */
    k = p;
    /* nums is not rising as a whole, so the loop stops before j < 0. */
    for (j = n - 1; j == n - 1 || nums[j] < nums[j + 1]; j--) {
        while (k >= 0 && nums[k] >= nums[j])
            k--;
        count += k + 2;
    }
    return count;
}
