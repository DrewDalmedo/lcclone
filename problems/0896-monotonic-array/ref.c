/* Reference solution: one pass, noting whether some step climbs and whether
 * some step drops; the answer is false as soon as both have happened. The
 * neighbours are compared directly, never by multiplying differences, which
 * could overflow. */
bool isMonotonic(int *nums, int numsSize)
{
    bool climbs = false, drops = false;
    int i;

    for (i = 1; i < numsSize; i++) {
        if (nums[i] > nums[i - 1])
            climbs = true;
        else if (nums[i] < nums[i - 1])
            drops = true;
        if (climbs && drops)
            return false;
    }
    return true;
}
