/* Slow and obviously right: for every possible digit sum (up to 45, more
 * than enough), count the numbers from 1 to n whose decimal text adds up
 * to it. Then take the largest of those counts, and count the digit sums
 * that reach it. */
static int digit_sum(int x)
{
    char text[16];
    int s = 0, i;

    sprintf(text, "%d", x);
    for (i = 0; text[i]; i++)
        s += text[i] - '0';
    return s;
}

int countLargestGroup(int n)
{
    int size[46], largest = 0, groups = 0, s, x;

    for (s = 0; s <= 45; s++) {
        size[s] = 0;
        for (x = 1; x <= n; x++)
            if (digit_sum(x) == s)
                size[s]++;
    }
    for (s = 0; s <= 45; s++)
        if (size[s] > largest)
            largest = size[s];
    for (s = 0; s <= 45; s++)
        if (size[s] == largest)
            groups++;
    return groups;
}
