/* Slow and obviously right: for each entry in order, compare it with every
 * other entry; if none is equal, it is the next one-off. */
char *kthDistinct(char **arr, int arrSize, int k)
{
    char *out;
    int i, j, alone;

    for (i = 0; i < arrSize; i++) {
        alone = 1;
        for (j = 0; j < arrSize; j++)
            if (j != i && strcmp(arr[i], arr[j]) == 0)
                alone = 0;
        if (alone && --k == 0) {
            out = malloc(strlen(arr[i]) + 1);
            strcpy(out, arr[i]);
            return out;
        }
    }
    out = malloc(1);
    out[0] = '\0';
    return out;
}
