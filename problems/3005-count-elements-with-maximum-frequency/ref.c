/* Reference solution: count every value in a table indexed by the value
 * (values go up to 100, so 101 slots), find the largest count, then add
 * up the counts equal to it. */
int maxFrequencyElements(int *nums, int numsSize)
{
    int count[101] = {0}, top = 0, total = 0, i;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    for (i = 1; i <= 100; i++)
        if (count[i] > top)
            top = count[i];
    for (i = 1; i <= 100; i++)
        if (count[i] == top)
            total += top;
    return total;
}
