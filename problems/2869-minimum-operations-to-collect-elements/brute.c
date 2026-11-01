/* Slow and obviously right: for s = 1, 2, ..., check whether the last s
 * elements of nums contain each of 1..k, searching for each one anew. */
int minOperations(int *nums, int numsSize, int k)
{
    int s, v, i, found;

    for (s = 1; s < numsSize; s++) {
        for (v = 1; v <= k; v++) {
            found = 0;
            for (i = numsSize - s; i < numsSize; i++)
                if (nums[i] == v)
                    found = 1;
            if (!found)
                break;
        }
        if (v > k)
            return s;
    }
    return numsSize;
}
