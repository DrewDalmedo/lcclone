/* Reference solution: sort, then spend flips on the negatives, from the
 * most negative up. Flips left over once no negative remains can go in
 * pairs to one element, which changes nothing; an odd one out goes to the
 * element of smallest absolute value and costs twice that value (nothing
 * if it is 0). */
static int by_value(const void *a, const void *b)
{
    int x = *(const int *)a, y = *(const int *)b;

    return (x > y) - (x < y);
}

int largestSumAfterKNegations(int *nums, int numsSize, int k)
{
    int sum = 0, smallest = 100, i;

    qsort(nums, (size_t)numsSize, sizeof *nums, by_value);
    for (i = 0; i < numsSize && k > 0 && nums[i] < 0; i++) {
        nums[i] = -nums[i];
        k--;
    }
    for (i = 0; i < numsSize; i++) {
        sum += nums[i];
        if (abs(nums[i]) < smallest)
            smallest = abs(nums[i]);
    }
    if (k % 2 == 1)
        sum -= 2 * smallest;
    return sum;
}
