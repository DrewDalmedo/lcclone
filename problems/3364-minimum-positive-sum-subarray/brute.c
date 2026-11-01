/* Slow and obviously right: for every allowed length and every start, add
 * the window up from scratch, and keep the smallest positive total. */
int minimumSumSubarray(int *nums, int numsSize, int l, int r)
{
    int found = 0, best = 0, len, start, i, sum;

    for (len = l; len <= r; len++)
        for (start = 0; start + len <= numsSize; start++) {
            sum = 0;
            for (i = start; i < start + len; i++)
                sum += nums[i];
            if (sum > 0 && (!found || sum < best)) {
                best = sum;
                found = 1;
            }
        }
    return found ? best : -1;
}
