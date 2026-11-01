/* Slow and obviously right: write each element out in decimal and add up
 * its digit characters. Every position is checked, from the back, so the
 * last match found is the lowest one. */
int smallestIndex(int *nums, int numsSize)
{
    char buf[16];
    int best = -1, i, sum;
    const char *p;

    for (i = numsSize - 1; i >= 0; i--) {
        snprintf(buf, sizeof buf, "%d", nums[i]);
        sum = 0;
        for (p = buf; *p; p++)
            sum += *p - '0';
        if (sum == i)
            best = i;
    }
    return best;
}
