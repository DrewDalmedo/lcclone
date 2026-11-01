/* Slow and obviously right: an entry's digit range is the largest gap
 * between any two of its digits, found by writing it out as text and
 * comparing every pair of characters. Then try each range from 9 down to
 * 0 and return the sum for the first one that some entry has. */
static int pair_range(int v)
{
    char text[16];
    int i, j, gap, best = 0;

    snprintf(text, sizeof text, "%d", v);
    for (i = 0; text[i] != '\0'; i++)
        for (j = 0; text[j] != '\0'; j++) {
            gap = text[i] - text[j];
            if (gap > best)
                best = gap;
        }
    return best;
}

int maxDigitRange(int *nums, int numsSize)
{
    int r, i, sum, found;

    for (r = 9; r >= 0; r--) {
        sum = 0;
        found = 0;
        for (i = 0; i < numsSize; i++)
            if (pair_range(nums[i]) == r) {
                sum += nums[i];
                found = 1;
            }
        if (found)
            return sum;
    }
    return 0;
}
