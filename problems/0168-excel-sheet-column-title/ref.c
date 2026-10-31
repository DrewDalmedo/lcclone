/* Reference solution: bijective base 26. Taking 1 off the number turns the
 * last letter into an ordinary base-26 digit from 0 (A) to 25 (Z); the
 * letters come out last first, so they are written from the end of the
 * buffer backwards. */
char *convertToTitle(int columnNumber)
{
    char buf[8];
    int pos = 7, n = columnNumber;
    char *out;

    buf[pos] = '\0';
    while (n > 0) {
        n--;
        buf[--pos] = (char)('A' + n % 26);
        n /= 26;
    }
    out = malloc((size_t)(8 - pos));
    memcpy(out, buf + pos, (size_t)(8 - pos));
    return out;
}
