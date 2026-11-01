/* Reference solution: mark which values occur in nums, then double x
 * while it is marked. Values above 1000 never occur, which also keeps
 * the lookup inside the table (x can reach 2000). */
int findFinalValue(int *nums, int numsSize, int original)
{
    bool present[1001] = {false};
    int x = original, i;

    for (i = 0; i < numsSize; i++)
        present[nums[i]] = true;
    while (x <= 1000 && present[x])
        x *= 2;
    return x;
}
