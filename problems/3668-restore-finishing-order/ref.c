/* Reference solution: mark the friends' bib numbers in a table indexed by
 * number (n + 1 slots, as numbers start at 1), then read the results sheet
 * from the top and keep every marked number. */
int *recoverOrder(int *order, int orderSize, int *friends, int friendsSize,
                  int *returnSize)
{
    char *known = calloc((size_t)orderSize + 1, 1);
    int *out = malloc((size_t)friendsSize * sizeof *out);
    int m = 0, i;

    for (i = 0; i < friendsSize; i++)
        known[friends[i]] = 1;
    for (i = 0; i < orderSize; i++)
        if (known[order[i]])
            out[m++] = order[i];
    free(known);
    *returnSize = m;
    return out;
}
