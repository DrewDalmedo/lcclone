/* Reference solution: one pass counts the copies of each value from 1 to
 * 100 and keeps the indices of its first three. A value counts when it
 * has exactly three copies and the middle one is as far from the first as
 * from the last. */
int countSpecialIntegers(int *nums, int numsSize)
{
    int count[101] = {0}, at[101][3], i, v, answer = 0;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        if (count[v] < 3)
            at[v][count[v]] = i;
        count[v]++;
    }
    for (v = 1; v <= 100; v++)
        if (count[v] == 3 && at[v][1] - at[v][0] == at[v][2] - at[v][1])
            answer++;
    return answer;
}
