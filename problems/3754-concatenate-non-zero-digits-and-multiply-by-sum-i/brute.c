/* Slow and obviously right: print n as text and read its characters from
 * left to right, appending each digit other than '0' to x. Then add up the
 * digits of x by peeling them off. */
long long sumAndMultiply(int n)
{
    char text[16];
    long long x = 0, rest, s = 0;
    int i;

    snprintf(text, sizeof text, "%d", n);
    for (i = 0; text[i] != '\0'; i++)
        if (text[i] != '0')
            x = x * 10 + (text[i] - '0');
    for (rest = x; rest > 0; rest /= 10)
        s += rest % 10;
    return x * s;
}
