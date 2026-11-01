/* Slow and obviously right: find the largest value, then the largest
 * value below it, then the largest value below that one, scanning the
 * whole array each time. */
int thirdMax(int *nums, int numsSize)
{
    int best[3], found, i, have;

    for (found = 0; found < 3; found++) {
        have = 0;
        for (i = 0; i < numsSize; i++)
            if ((found == 0 || nums[i] < best[found - 1]) &&
                (!have || nums[i] > best[found])) {
                best[found] = nums[i];
                have = 1;
            }
        if (!have)
            break;
    }
    return found == 3 ? best[2] : best[0];
}
