/* Slow and obviously right: write n in binary as text, write that text
 * backwards as a second string, and count the positions where they differ. */
int minimumFlips(int n)
{
    char low_first[32], s[32], mirror[32];
    int len = 0, i, count = 0;

    for (; n > 0; n /= 2)
        low_first[len++] = (char)('0' + n % 2);
    for (i = 0; i < len; i++)
        s[i] = low_first[len - 1 - i];
    for (i = 0; i < len; i++)
        mirror[i] = s[len - 1 - i];
    for (i = 0; i < len; i++)
        if (s[i] != mirror[i])
            count++;
    return count;
}
