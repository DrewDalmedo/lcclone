/* Reference solution: walk from the back, marking the codes seen. The
 * first code met a second time is at the latest position p that has a
 * copy behind it; the belt is clear exactly once that parcel is gone,
 * which takes p / 3 + 1 sweeps. With no repeat at all, 0. */
int minimumOperations(int *nums, int numsSize)
{
    char seen[101] = {0};
    int i;

    for (i = numsSize - 1; i >= 0; i--) {
        if (seen[nums[i]])
            return i / 3 + 1;
        seen[nums[i]] = 1;
    }
    return 0;
}
