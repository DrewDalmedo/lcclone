/* Reference solution: pair each height with its name, sort the pairs by
 * height from the tallest down, and read off the names. The returned
 * array points at the caller's strings. */
struct singer {
    int height;
    char *name;
};

static int taller_first(const void *x, const void *y)
{
    const struct singer *a = x, *b = y;

    return b->height - a->height;
}

char **sortPeople(char **names, int namesSize, int *heights, int heightsSize,
                  int *returnSize)
{
    struct singer *row = malloc((size_t)namesSize * sizeof *row);
    char **out = malloc((size_t)namesSize * sizeof *out);
    int i;

    (void)heightsSize;
    for (i = 0; i < namesSize; i++) {
        row[i].height = heights[i];
        row[i].name = names[i];
    }
    qsort(row, (size_t)namesSize, sizeof *row, taller_first);
    for (i = 0; i < namesSize; i++)
        out[i] = row[i].name;
    free(row);
    *returnSize = namesSize;
    return out;
}
