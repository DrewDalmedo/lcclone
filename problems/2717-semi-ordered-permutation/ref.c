/* Reference solution: with 1 at index a and n at index b, 1 needs a swaps
 * to reach the front and n needs n - 1 - b to reach the back. When 1
 * starts right of n, one swap exchanges the two and counts for both. */
int semiOrderedPermutation(int *nums, int numsSize)
{
    int n = numsSize, a = 0, b = 0, i;

    for (i = 0; i < n; i++) {
        if (nums[i] == 1)
            a = i;
        if (nums[i] == n)
            b = i;
    }
    return a + (n - 1 - b) - (a > b ? 1 : 0);
}
