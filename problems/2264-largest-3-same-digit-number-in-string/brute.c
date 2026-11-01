/* Slow and obviously right: try the candidates from "999" down to "000",
 * and return the first one that occurs in num as a substring. */
char *largestGoodInteger(char *num)
{
    char *out = malloc(4);
    int d;

    for (d = 9; d >= 0; d--) {
        out[0] = out[1] = out[2] = (char)('0' + d);
        out[3] = '\0';
        if (strstr(num, out))
            return out;
    }
    out[0] = '\0';
    return out;
}
