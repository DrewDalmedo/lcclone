/* Reference solution: scatter. Copy each letter s[i] straight to its
 * place indices[i] in a new string of n + 1 bytes, then end it with a
 * '\0'. */
char *restoreString(char *s, int *indices, int indicesSize)
{
    char *out = malloc((size_t)indicesSize + 1);
    int i;

    for (i = 0; i < indicesSize; i++)
        out[indices[i]] = s[i];
    out[indicesSize] = '\0';
    return out;
}
