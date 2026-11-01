/* Slow and obviously right: fill the new string position by position,
 * searching indices for the letter that is sent to each one. */
char *restoreString(char *s, int *indices, int indicesSize)
{
    char *out = malloc((size_t)indicesSize + 1);
    int pos, i;

    for (pos = 0; pos < indicesSize; pos++)
        for (i = 0; i < indicesSize; i++)
            if (indices[i] == pos)
                out[pos] = s[i];
    out[indicesSize] = '\0';
    return out;
}
