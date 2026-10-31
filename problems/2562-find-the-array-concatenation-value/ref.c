/* Reference solution: walk two indices in from both ends. Concatenating
 * a and b is a * 10^d + b, where 10^d is the smallest power of ten above
 * b; the middle element of an odd-length array is added as it is. */
long long findTheArrayConcVal(int *nums, int numsSize)
{
    long long total = 0, p;
    int i = 0, j = numsSize - 1;

    while (i < j) {
        for (p = 10; p <= nums[j]; p *= 10)
            ;
        total += nums[i] * p + nums[j];
        i++;
        j--;
    }
    if (i == j)
        total += nums[i];
    return total;
}
