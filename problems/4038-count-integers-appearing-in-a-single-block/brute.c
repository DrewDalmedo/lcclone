/* Slow and obviously right: for each value, at its first occurrence, find
 * its last occurrence and check that every entry in between equals it. */
int countSpecialIntegers(int *nums, int numsSize)
{
    int i, j, last, seen, ok, count = 0;

    for (i = 0; i < numsSize; i++) {
        seen = 0;
        for (j = 0; j < i; j++)
            if (nums[j] == nums[i])
                seen = 1;
        if (seen)
            continue;
        last = i;
        for (j = i; j < numsSize; j++)
            if (nums[j] == nums[i])
                last = j;
        ok = 1;
        for (j = i; j <= last; j++)
            if (nums[j] != nums[i])
                ok = 0;
        count += ok;
    }
    return count;
}
