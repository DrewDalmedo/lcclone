/* Slow and obviously right: print n as text, reverse the characters, and
 * read the reversed text back as a number (leading zeros drop out). */
int mirrorDistance(int n)
{
    char text[16];
    long long back = 0, d;
    int len = sprintf(text, "%d", n), i;

    for (i = len - 1; i >= 0; i--)
        back = back * 10 + (text[i] - '0');
    d = (long long)n - back;
    return (int)(d < 0 ? -d : d);
}
