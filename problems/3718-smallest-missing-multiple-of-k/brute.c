/* Slow and obviously right: try k, 2k, 3k, ... in turn, searching the
 * whole array for each, and return the first one that is not found. */
int missingMultiple(int *nums, int numsSize, int k)
{
    int m, found, i;

    for (m = k;; m += k) {
        found = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == m)
                found = 1;
        if (!found)
            return m;
    }
}
