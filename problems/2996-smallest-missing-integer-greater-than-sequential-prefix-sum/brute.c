/* Slow and obviously right: find how long the sequential prefix is and add
 * it up, then try s, s + 1, s + 2, ... in turn, searching all of nums for
 * each, until one is not found. */
int missingInteger(int *nums, int numsSize)
{
    int len = 1, s = 0, x, i, found;

    while (len < numsSize && nums[len] - nums[len - 1] == 1)
        len++;
    for (i = 0; i < len; i++)
        s += nums[i];
    for (x = s;; x++) {
        found = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == x)
                found = 1;
        if (!found)
            return x;
    }
}
