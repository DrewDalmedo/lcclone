/* Reference solution: for each of the 31 bit positions an int can use,
 * count the elements that have that bit, and keep the bit when the count
 * reaches k. */
int findKOr(int *nums, int numsSize, int k)
{
    int result = 0, b, i, votes;

    for (b = 0; b < 31; b++) {
        votes = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] >> b & 1)
                votes++;
        if (votes >= k)
            result |= 1 << b;
    }
    return result;
}
