/* Slow and obviously right: for each leg's destination, look through every
 * leg for one that sets out from it; return the destination none does. */
char *destCity(char ***paths, int pathsSize, int *pathsColSize)
{
    int i, k, leaves;
    char *out;

    (void)pathsColSize;
    for (i = 0; i < pathsSize; i++) {
        leaves = 0;
        for (k = 0; k < pathsSize; k++)
            if (strcmp(paths[i][1], paths[k][0]) == 0)
                leaves = 1;
        if (!leaves) {
            out = malloc(strlen(paths[i][1]) + 1);
            strcpy(out, paths[i][1]);
            return out;
        }
    }
    return NULL;
}
