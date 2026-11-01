/* Slow and obviously right: try every distance from 0 upwards, and at the
 * first one where some element lies, prefer +d to -d. */
int findClosestNumber(int *nums, int numsSize)
{
    int d, i, neg;

    for (d = 0;; d++) {
        neg = 0;
        for (i = 0; i < numsSize; i++) {
            if (nums[i] == d)
                return d;
            if (nums[i] == -d)
                neg = 1;
        }
        if (neg)
            return -d;
    }
}
