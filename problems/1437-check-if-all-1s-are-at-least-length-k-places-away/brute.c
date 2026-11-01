/* Slow and obviously right: for every pair of 1s, count the zeros between
 * them one by one. A pair that isn't consecutive has at least as many
 * zeros between it as the consecutive pair it starts with, so checking
 * every pair gives the same answer. */
bool kLengthApart(int *nums, int numsSize, int k)
{
    int i, j, t, zeros;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++) {
            if (nums[i] != 1 || nums[j] != 1)
                continue;
            zeros = 0;
            for (t = i + 1; t < j; t++)
                if (nums[t] == 0)
                    zeros++;
            if (zeros < k)
                return false;
        }
    return true;
}
