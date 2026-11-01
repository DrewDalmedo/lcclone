/* Obviously right by another route, with no product of three sides: the
 * volume reaches 10^9 exactly when height is at least 10^9 divided by
 * length * width, rounded up. Each side is checked on its own, and the
 * label is looked up in a table by the two answers. */
char *categorizeBox(int length, int width, int height, int mass)
{
    static const char *labels[2][2] = {{"Neither", "Heavy"},
                                       {"Bulky", "Both"}};
    long long base = (long long)length * width;
    long long need = (1000000000LL + base - 1) / base;
    int sides[3], bulky = 0, heavy = mass >= 100, i;
    char *out;

    sides[0] = length;
    sides[1] = width;
    sides[2] = height;
    for (i = 0; i < 3; i++)
        if (sides[i] >= 10000)
            bulky = 1;
    if (height >= need)
        bulky = 1;
    out = malloc(8);
    strcpy(out, labels[bulky][heavy]);
    return out;
}
