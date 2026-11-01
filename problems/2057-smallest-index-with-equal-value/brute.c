/* Slow and obviously right: go through nums from the back, working out the
 * counter's digit for each index by taking 10 away until it is below 10,
 * and remember the last agreeing index seen, which is the smallest one. */
int smallestEqual(int *nums, int numsSize)
{
    int best = -1, i, digit;

    for (i = numsSize - 1; i >= 0; i--) {
        digit = i;
        while (digit >= 10)
            digit -= 10;
        if (nums[i] == digit)
            best = i;
    }
    return best;
}
