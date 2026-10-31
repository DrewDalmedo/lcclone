/* Slow and obviously right: each round, scan for the largest pile and
 * replace it by the largest r with r * r <= c, found by counting r up. */
long long pickGifts(int *gifts, int giftsSize, int k)
{
    long long *pile = malloc((size_t)giftsSize * sizeof *pile);
    long long total = 0, r;
    int round, i, big;

    for (i = 0; i < giftsSize; i++)
        pile[i] = gifts[i];
    for (round = 0; round < k; round++) {
        big = 0;
        for (i = 1; i < giftsSize; i++)
            if (pile[i] > pile[big])
                big = i;
        r = 0;
        while ((r + 1) * (r + 1) <= pile[big])
            r++;
        pile[big] = r;
    }
    for (i = 0; i < giftsSize; i++)
        total += pile[i];
    free(pile);
    return total;
}
