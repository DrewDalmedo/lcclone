/* Slow and obviously right: bubble each zero towards the back by swapping
 * it with a non-zero right after it, until no such pair is left. Only
 * neighbours of which one is zero are swapped, so the non-zero values never
 * pass each other. */
void moveZeroes(int *nums, int numsSize)
{
    int changed = 1, i, t;

    while (changed) {
        changed = 0;
        for (i = 0; i + 1 < numsSize; i++)
            if (nums[i] == 0 && nums[i + 1] != 0) {
                t = nums[i];
                nums[i] = nums[i + 1];
                nums[i + 1] = t;
                changed = 1;
            }
    }
}
