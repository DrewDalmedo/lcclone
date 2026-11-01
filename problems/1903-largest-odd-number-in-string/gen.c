/* Small cases have 1 to 8 digits, each odd with a chance the seed picks (0,
 * 20, 50 or 80 in 100), and even digits are often zeros, so that strings
 * with no odd digit, an odd digit only at the start, and zeros after the
 * last odd digit come up often. Full-size ones have 10^5 digits, the seed
 * picking where the last odd digit goes: a little before the end, nowhere,
 * at the start, somewhere in the middle, or at the very end. Before it the
 * digits are random; after it they are all even. The first digit is never
 * 0. */
static char digit(int odd, int first)
{
    if (odd)
        return rnd_char("13579");
    return first ? rnd_char("2468") : rnd_char("002468");
}

void gen(int small)
{
    static const int chances[] = {0, 20, 50, 80};
    int n = small ? (int)rnd(1, 8) : 100000;
    int chance = 0, last = -1, shape, odd, i;
    char *num = malloc((size_t)n + 1);

    if (small) {
        chance = chances[rnd(0, 3)];
    } else {
        shape = (int)rnd(0, 4);
        if (shape == 0)
            last = n - 1 - (int)rnd(1, 40);
        else if (shape == 1)
            last = -1;
        else if (shape == 2)
            last = 0;
        else if (shape == 3)
            last = (int)rnd(1, n - 2);
        else
            last = n - 1;
    }
    for (i = 0; i < n; i++) {
        if (small)
            odd = rnd_chance(chance);
        else if (i < last)
            odd = rnd_chance(50);
        else
            odd = i == last;
        num[i] = digit(odd, i == 0);
    }
    num[n] = '\0';
    put_string(num);
    free(num);
}
