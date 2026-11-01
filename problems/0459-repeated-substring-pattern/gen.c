/* Small cases: up to 15 letters from a, b (sometimes c). The seed picks
 * the shape: random letters; a block of 1 to 4 letters written 2 or more
 * times, as it is, with one letter changed, or with the last copy cut short
 * or followed by part of another; copies of a block with their letters
 * shuffled; or one letter throughout, maybe with a different last letter.
 *
 * Full-size cases have 10^4 letters, or 7560 or 9240, the lengths up to
 * 10^4 with the most divisors (64). Shapes: one letter throughout but for a
 * different last letter (every block length fails only at the very end);
 * a block whose length divides n written n / d times, the block itself
 * maybe a repetition, and maybe with the last letter changed; a block whose
 * length doesn't divide n repeated until n letters; or copies of a block
 * where the last copy is rotated by one place. */
static void fill(char *s, int len, const char *letters)
{
    int i;

    for (i = 0; i < len; i++)
        s[i] = rnd_char(letters);
}

/* a random divisor of n below n, at least lo (n must have one) */
static int divisor(int n, int lo)
{
    int d[128], cnt = 0, i;

    for (i = lo; i < n; i++)
        if (n % i == 0)
            d[cnt++] = i;
    return d[rnd(0, cnt - 1)];
}

static void small_case(char *s)
{
    const char *letters = rnd_chance(70) ? "ab" : "abc";
    int shape = (int)rnd(0, 5), n, d, k, i, j, r;
    char unit[4], c;
    int order[4];

    if (shape == 0) {
        n = (int)rnd(1, 10);
        fill(s, n, letters);
    } else if (shape <= 3) {
        d = (int)rnd(1, 4);
        k = (int)rnd(2, 12 / d);
        fill(unit, d, letters);
        n = d * k;
        for (i = 0; i < n; i++)
            s[i] = unit[i % d];
        if (shape == 2) {
            i = (int)rnd(0, n - 1);
            c = rnd_char(letters);
            s[i] = c;
        } else if (shape == 3 && d >= 2) {
            r = (int)rnd(1, d - 1);
            if (rnd_chance(50))
                n -= r;
            else
                for (i = 0; i < r; i++, n++)
                    s[n] = unit[n % d];
        } else if (shape == 3) {
            c = s[n - 1] == 'a' ? 'b' : 'a';
            s[n - 1] = c;
        }
    } else if (shape == 4) {
        d = (int)rnd(2, 4);
        k = (int)rnd(2, 3);
        fill(unit, d, letters);
        n = 0;
        for (j = 0; j < k; j++) {
            for (i = 0; i < d; i++)
                order[i] = i;
            rnd_shuffle(order, d);
            for (i = 0; i < d; i++)
                s[n++] = unit[order[i]];
        }
    } else {
        n = (int)rnd(1, 12);
        c = rnd_char(letters);
        for (i = 0; i < n; i++)
            s[i] = c;
        if (n >= 2 && rnd_chance(50))
            s[n - 1] = c == 'a' ? 'b' : 'a';
    }
    s[n] = '\0';
}

static void large_case(char *s)
{
    static const int sizes[] = {10000, 7560, 9240};
    const char *letters = rnd_chance(50) ? "ab" : "abcdefghijklmnopqrstuvwxyz";
    int n = sizes[rnd(0, 2)], shape = (int)rnd(0, 4), d, e, i;
    char *unit = malloc((size_t)n + 1), c;

    if (shape == 0) {
        c = rnd_char("abcdefghijklmnopqrstuvwxyz");
        memset(s, c, (size_t)n);
        s[n - 1] = c == 'z' ? 'a' : (char)(c + 1);
    } else if (shape <= 2) {
        d = divisor(n, 1);
        if (d >= 2 && rnd_chance(50)) {
            e = divisor(d, 1);
            fill(unit, e, letters);
            for (i = e; i < d; i++)
                unit[i] = unit[i - e];
        } else
            fill(unit, d, letters);
        for (i = 0; i < n; i++)
            s[i] = unit[i % d];
        if (shape == 2)
            s[n - 1] = s[n - 1] == 'a' ? 'b' : 'a';
    } else if (shape == 3) {
        n = 10000;
        do
            d = (int)rnd(3, 6000);
        while (n % d == 0);
        fill(unit, d, letters);
        for (i = 0; i < n; i++)
            s[i] = unit[i % d];
    } else {
        d = divisor(n, 2);
        fill(unit, d, letters);
        unit[0] = 'a';
        unit[d - 1] = 'b';
        for (i = 0; i < n - d; i++)
            s[i] = unit[i % d];
        for (i = 0; i < d; i++)
            s[n - d + i] = unit[(i + 1) % d];
    }
    s[n] = '\0';
    free(unit);
}

void gen(int small)
{
    char *s = malloc(10001);

    if (small)
        small_case(s);
    else
        large_case(s);
    put_string(s);
    free(s);
}
