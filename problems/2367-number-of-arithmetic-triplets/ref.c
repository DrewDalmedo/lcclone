/* Reference solution: the values are distinct, so a ladder is fixed by its
 * lowest value v, and exists exactly when v + diff and v + 2 * diff are in
 * nums too. Mark the values in a table, sized for v + 2 * diff up to 300,
 * and count the values whose two partners are marked. */
int arithmeticTriplets(int *nums, int numsSize, int diff)
{
    char present[301] = {0};
    int count = 0, i;

    for (i = 0; i < numsSize; i++)
        present[nums[i]] = 1;
    for (i = 0; i < numsSize; i++)
        if (present[nums[i] + diff] && present[nums[i] + 2 * diff])
            count++;
    return count;
}
