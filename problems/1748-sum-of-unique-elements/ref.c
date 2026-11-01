/* Reference solution: count how often each value occurs in a table
 * indexed by value (1..100), then add up the values counted once. */
int sumOfUnique(int *nums, int numsSize)
{
    int count[101] = {0};
    int sum = 0, i, v;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (v = 1; v <= 100; v++)
        if (count[v] == 1)
            sum += v;
    return sum;
}
