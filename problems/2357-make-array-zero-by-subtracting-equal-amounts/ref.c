/* Reference solution: a round lowers all positive entries alike, so equal
 * ones stay equal and different ones stay different, and only those equal
 * to the smallest positive value can reach 0 in it (when x is that value).
 * So each round removes at most one distinct positive value, and choosing
 * x = m removes exactly one: the answer is the number of distinct positive
 * values, counted with a table indexed by value. */
int minimumOperations(int *nums, int numsSize)
{
    char seen[101] = {0};
    int count = 0, i;

    for (i = 0; i < numsSize; i++)
        if (nums[i] > 0 && !seen[nums[i]]) {
            seen[nums[i]] = 1;
            count++;
        }
    return count;
}
