/* Straight from the definition: for every value from 0 to 50, count the
 * window positions that hold at least one copy of it, and keep the
 * largest value counted exactly once. */
int largestInteger(int *nums, int numsSize, int k)
{
    int best = -1, v, start, i, count, found;

    for (v = 0; v <= 50; v++) {
        count = 0;
        for (start = 0; start + k <= numsSize; start++) {
            found = 0;
            for (i = start; i < start + k; i++)
                if (nums[i] == v)
                    found = 1;
            count += found;
        }
        if (count == 1)
            best = v;
    }
    return best;
}
