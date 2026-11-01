/* Slow and obviously right: write x out as text to add up its digits,
 * then look for a multiple of the total that equals x. */
int sumOfTheDigitsOfHarshadNumber(int x)
{
    char text[16];
    int s = 0, i, m;

    snprintf(text, sizeof text, "%d", x);
    for (i = 0; text[i] != '\0'; i++)
        s += text[i] - '0';
    for (m = 1; m * s <= x; m++)
        if (m * s == x)
            return s;
    return -1;
}
