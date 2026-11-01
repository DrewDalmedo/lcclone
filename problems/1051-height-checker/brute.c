/* Slow and obviously right: sort a copy of heights with insertion sort and
 * compare it with the original position by position. */
int heightChecker(int *heights, int heightsSize)
{
    int *tidy = malloc((size_t)heightsSize * sizeof *tidy);
    int changed = 0, i, j, v;

    for (i = 0; i < heightsSize; i++)
        tidy[i] = heights[i];
    for (i = 1; i < heightsSize; i++) {
        v = tidy[i];
        for (j = i; j > 0 && tidy[j - 1] > v; j--)
            tidy[j] = tidy[j - 1];
        tidy[j] = v;
    }
    for (i = 0; i < heightsSize; i++)
        if (heights[i] != tidy[i])
            changed++;
    free(tidy);
    return changed;
}
