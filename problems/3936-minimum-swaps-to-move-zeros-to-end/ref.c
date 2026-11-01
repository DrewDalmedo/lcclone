/* Reference solution: with z zeros, a settled array has them in the last z
 * positions. Every zero among the first n - z positions has to be moved,
 * and one swap can bring at most one zero into the back block; swapping it
 * with a non-zero value from the back block does exactly that. So the
 * answer is the number of zeros among the first n - z positions. */
int minimumSwaps(int *nums, int numsSize)
{
    int zeros = 0, misplaced = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] == 0)
            zeros++;
    for (i = 0; i < numsSize - zeros; i++)
        if (nums[i] == 0)
            misplaced++;
    return misplaced;
}
