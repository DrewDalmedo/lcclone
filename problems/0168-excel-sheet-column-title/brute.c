/* Slow and obviously right, straight from the order of the names: skip
 * whole lengths (26 names of one letter, 26^2 of two, ...) until the
 * column falls inside one, then its place among the names of that length,
 * in alphabetical order, is an ordinary base-26 number with A as 0,
 * padded with A to the full length. */
char *convertToTitle(int columnNumber)
{
    long long rest = columnNumber, count = 26;
    int len = 1, i;
    char *out;

    while (rest > count) {
        rest -= count;
        count *= 26;
        len++;
    }
    rest--;
    out = malloc((size_t)len + 1);
    for (i = len - 1; i >= 0; i--) {
        out[i] = (char)('A' + rest % 26);
        rest /= 26;
    }
    out[len] = '\0';
    return out;
}
