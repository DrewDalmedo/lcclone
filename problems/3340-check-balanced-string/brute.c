/* Slow and obviously right, by another route: add up all the digits, and
 * separately the digits on light squares (even indices). The two shades
 * balance exactly when the light total is half of the whole. */
bool isBalanced(char *num)
{
    int total = 0, light = 0, i;
    int n = (int)strlen(num);

    for (i = 0; i < n; i++)
        total += num[i] - '0';
    for (i = 0; i < n; i += 2)
        light += num[i] - '0';
    return 2 * light == total;
}
