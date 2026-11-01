/* Reference solution: one pass over the windows of three neighbouring
 * characters, remembering the highest digit that fills a whole window.
 * The best digit starts at -1, so that a triple of zeros still counts. */
char *largestGoodInteger(char *num)
{
    int n = (int)strlen(num), best = -1, i;
    char *out = malloc(4);

    for (i = 0; i + 2 < n; i++)
        if (num[i] == num[i + 1] && num[i] == num[i + 2] &&
            num[i] - '0' > best)
            best = num[i] - '0';
    if (best < 0) {
        out[0] = '\0';
    } else {
        out[0] = out[1] = out[2] = (char)('0' + best);
        out[3] = '\0';
    }
    return out;
}
