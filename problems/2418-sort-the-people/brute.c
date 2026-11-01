/* Slow and obviously right: fill the row one place at a time, each time
 * picking the tallest singer not placed yet, and copy that singer's name. */
char **sortPeople(char **names, int namesSize, int *heights, int heightsSize,
                  int *returnSize)
{
    char **out = malloc((size_t)namesSize * sizeof *out);
    char *placed = calloc((size_t)namesSize, 1);
    int slot, i, best;

    (void)heightsSize;
    for (slot = 0; slot < namesSize; slot++) {
        best = -1;
        for (i = 0; i < namesSize; i++)
            if (!placed[i] && (best < 0 || heights[i] > heights[best]))
                best = i;
        placed[best] = 1;
        out[slot] = malloc(strlen(names[best]) + 1);
        strcpy(out[slot], names[best]);
    }
    free(placed);
    *returnSize = namesSize;
    return out;
}
