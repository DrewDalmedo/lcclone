/* Reference solution: one pass, remembering where the last 1 was; each new
 * 1 must have at least k zeros between it and that one. */
bool kLengthApart(int *nums, int numsSize, int k)
{
    int last = -1, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] == 1) {
            if (last >= 0 && i - last - 1 < k)
                return false;
            last = i;
        }
    return true;
}
