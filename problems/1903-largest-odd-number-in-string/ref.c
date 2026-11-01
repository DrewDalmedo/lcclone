/* Reference solution: an odd piece ends at an odd digit, so at or before
 * the rightmost one. The prefix of num that ends there is the longest such
 * piece and starts with a non-zero digit, so it beats every other. Scan
 * from the right for the last odd digit and return that prefix ("" if
 * there is no odd digit). */
char *largestOddNumber(char *num)
{
    size_t len = strlen(num);
    char *out;

    while (len > 0 && (num[len - 1] - '0') % 2 == 0)
        len--;
    out = malloc(len + 1);
    memcpy(out, num, len);
    out[len] = '\0';
    return out;
}
