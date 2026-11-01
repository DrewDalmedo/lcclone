/* Slow and obviously right: for each child, give it the bag and compare
 * its new total with every other child's count. */
bool *kidsWithCandies(int *candies, int candiesSize, int extraCandies,
                      int *returnSize)
{
    bool *answer = malloc((size_t)candiesSize * sizeof *answer);
    int i, j;

    for (i = 0; i < candiesSize; i++) {
        answer[i] = true;
        for (j = 0; j < candiesSize; j++)
            if (j != i && candies[i] + extraCandies < candies[j])
                answer[i] = false;
    }
    *returnSize = candiesSize;
    return answer;
}
