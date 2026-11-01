/* Slow and obviously right: write the loop out three times in a row, so
 * that every triple of the loop appears as three neighbouring entries
 * around the middle copy, and test the n triples centred there. */
int numberOfAlternatingGroups(int *colors, int colorsSize)
{
    int n = colorsSize, count = 0, i, j;
    int *row = malloc((size_t)(3 * n) * sizeof *row);

    for (i = 0; i < 3; i++)
        for (j = 0; j < n; j++)
            row[i * n + j] = colors[j];
    for (j = n; j < 2 * n; j++)
        if (row[j - 1] != row[j] && row[j] != row[j + 1])
            count++;
    free(row);
    return count;
}
