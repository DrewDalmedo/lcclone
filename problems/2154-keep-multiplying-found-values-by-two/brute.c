/* Slow and obviously right: search the whole array for the current value
 * each time, and double it whenever it is found. */
int findFinalValue(int *nums, int numsSize, int original)
{
    int x = original, found, i;

    for (;;) {
        found = 0;
        for (i = 0; i < numsSize; i++)
            if (nums[i] == x)
                found = 1;
        if (!found)
            return x;
        x *= 2;
    }
}
