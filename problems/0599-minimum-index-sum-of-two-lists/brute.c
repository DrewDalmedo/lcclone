/* Slow and obviously right: compare every string of list1 with every
 * string of list2. A first pass finds the smallest index sum of a match,
 * a second collects every match with that sum. */
char **findRestaurant(char **list1, int list1Size, char **list2,
                      int list2Size, int *returnSize)
{
    char **out = malloc((size_t)list1Size * sizeof *out);
    int best = -1, n = 0, i, j;

    for (i = 0; i < list1Size; i++)
        for (j = 0; j < list2Size; j++)
            if (strcmp(list1[i], list2[j]) == 0 && (best < 0 || i + j < best))
                best = i + j;
    for (i = 0; i < list1Size; i++)
        for (j = 0; j < list2Size; j++)
            if (strcmp(list1[i], list2[j]) == 0 && i + j == best) {
                out[n] = malloc(strlen(list1[i]) + 1);
                strcpy(out[n], list1[i]);
                n++;
            }
    *returnSize = n;
    return out;
}
