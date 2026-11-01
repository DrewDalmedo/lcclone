/* Reference solution: count the dots, allocate room for two more
 * characters per dot, and copy address across, writing "[.]" for each
 * dot. */
char *defangIPaddr(char *address)
{
    size_t n = strlen(address), dots = 0, i, k = 0;
    char *out;

    for (i = 0; i < n; i++)
        if (address[i] == '.')
            dots++;
    out = malloc(n + 2 * dots + 1);
    for (i = 0; i < n; i++) {
        if (address[i] == '.') {
            out[k++] = '[';
            out[k++] = '.';
            out[k++] = ']';
        } else {
            out[k++] = address[i];
        }
    }
    out[k] = '\0';
    return out;
}
