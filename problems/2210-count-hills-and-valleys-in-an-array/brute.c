/* Slow and obviously right: for the first element of each plateau, walk
 * outwards to the nearest element of a different height on each side, and
 * count the plateau when both exist and are both lower or both higher. */
int countHillValley(int *nums, int numsSize)
{
    int count = 0, i, l, r;

    for (i = 0; i < numsSize; i++) {
        if (i > 0 && nums[i - 1] == nums[i])
            continue;
        for (l = i - 1; l >= 0 && nums[l] == nums[i]; l--)
            ;
        for (r = i + 1; r < numsSize && nums[r] == nums[i]; r++)
            ;
        if (l < 0 || r >= numsSize)
            continue;
        if ((nums[l] < nums[i] && nums[r] < nums[i])
            || (nums[l] > nums[i] && nums[r] > nums[i]))
            count++;
    }
    return count;
}
