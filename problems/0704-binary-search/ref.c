/* Reference solution: binary search over the indices lo..hi (both
 * included) where target could still be. Each step compares the middle
 * element with target and drops the half that can't hold it. */
int search(int *nums, int numsSize, int target)
{
    int lo = 0, hi = numsSize - 1;

    while (lo <= hi) {
        int mid = lo + (hi - lo) / 2;

        if (nums[mid] == target)
            return mid;
        if (nums[mid] < target)
            lo = mid + 1;
        else
            hi = mid - 1;
    }
    return -1;
}
