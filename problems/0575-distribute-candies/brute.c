/* Slow and obviously right: try every choice of n / 2 candies (each subset
 * of the positions, as a bit mask, that has n / 2 members), count the
 * different kinds in it by comparing each chosen candy with the chosen
 * ones before it, and keep the largest count. Only for small n. */
int distributeCandies(int *candyType, int candyTypeSize)
{
    int n = candyTypeSize, best = 0, kinds, taken, i, j, dup;
    unsigned long mask;

    for (mask = 0; mask < 1UL << n; mask++) {
        taken = 0;
        for (i = 0; i < n; i++)
            if (mask >> i & 1)
                taken++;
        if (taken != n / 2)
            continue;
        kinds = 0;
        for (i = 0; i < n; i++) {
            if (!(mask >> i & 1))
                continue;
            dup = 0;
            for (j = 0; j < i; j++)
                if ((mask >> j & 1) && candyType[j] == candyType[i])
                    dup = 1;
            if (!dup)
                kinds++;
        }
        if (kinds > best)
            best = kinds;
    }
    return best;
}
