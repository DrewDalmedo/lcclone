/* Reference solution: no factor is negative, so the best pair is the two
 * largest entries, counted by position (a largest value that occurs twice
 * fills both places). Find them in one pass; an entry that ties the
 * largest so far pushes the old largest down to second place. */
int maxProduct(int *nums, int numsSize)
{
    int first = 0, second = 0, i;

    for (i = 0; i < numsSize; i++) {
        if (nums[i] >= first) {
            second = first;
            first = nums[i];
        } else if (nums[i] > second) {
            second = nums[i];
        }
    }
    return (first - 1) * (second - 1);
}
