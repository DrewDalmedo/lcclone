/* Slow and obviously right: for each index but the last, add up the
 * elements to its right anew, divide to get their average, and compare. */
int dominantIndices(int *nums, int numsSize)
{
    int count = 0, i, j;
    double sum, average;

    for (i = 0; i + 1 < numsSize; i++) {
        sum = 0;
        for (j = i + 1; j < numsSize; j++)
            sum += nums[j];
        average = sum / (numsSize - 1 - i);
        if (nums[i] > average)
            count++;
    }
    return count;
}
