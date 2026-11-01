/* Reference solution: the n / 2 candies taken can hold at most n / 2 kinds,
 * and at most every kind in the jar; one candy of each kind, topped up
 * with any others, reaches the smaller of the two. So count the different
 * kinds, marking each in a table that covers every label from -10^5 to
 * 10^5, and stop once there are n / 2 of them. */
int distributeCandies(int *candyType, int candyTypeSize)
{
    char *seen = calloc(200001, 1);
    int half = candyTypeSize / 2, kinds = 0, i;

    for (i = 0; i < candyTypeSize && kinds < half; i++)
        if (!seen[candyType[i] + 100000]) {
            seen[candyType[i] + 100000] = 1;
            kinds++;
        }
    free(seen);
    return kinds;
}
