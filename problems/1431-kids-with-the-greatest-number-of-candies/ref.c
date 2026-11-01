/* Reference solution: find the largest count M; child i can reach the top
 * exactly when candies[i] + extraCandies >= M. */
bool *kidsWithCandies(int *candies, int candiesSize, int extraCandies,
                      int *returnSize)
{
    bool *answer = malloc((size_t)candiesSize * sizeof *answer);
    int most = 0, i;

    for (i = 0; i < candiesSize; i++)
        if (candies[i] > most)
            most = candies[i];
    for (i = 0; i < candiesSize; i++)
        answer[i] = candies[i] + extraCandies >= most;
    *returnSize = candiesSize;
    return answer;
}
