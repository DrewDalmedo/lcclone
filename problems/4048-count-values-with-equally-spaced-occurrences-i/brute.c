/* Slow and obviously right: for each index holding a value for the first
 * time, list every index holding that value, and test the list. */
int countSpecialIntegers(int *nums, int numsSize)
{
    int i, j, m, seen, answer = 0;
    int *at = malloc((size_t)numsSize * sizeof *at);

    for (i = 0; i < numsSize; i++) {
        seen = 0;
        for (j = 0; j < i; j++)
            if (nums[j] == nums[i])
                seen = 1;
        if (seen)
            continue;
        m = 0;
        for (j = i; j < numsSize; j++)
            if (nums[j] == nums[i])
                at[m++] = j;
        if (m == 3 && 2 * at[1] == at[0] + at[2])
            answer++;
    }
    free(at);
    return answer;
}
