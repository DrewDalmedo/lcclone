/* A digit string. Random strings fold to two equal digits only about one
 * time in ten, so about half the cases are made true on purpose: fold the
 * string with its last digit set to 0, then pick the last digit that makes
 * the second digit left equal the first (the last digit adds exactly once
 * to the second and not at all to the first).
 *
 * Small cases have 3 to 10 digits, drawn from a set the seed picks (all
 * ten digits, or a few, so repeats and wraparounds are common). Full-size
 * ones have 100 digits, in a shape the seed picks: random, made true;
 * random; heavy in 9s, made true; or one digit repeated. */

static void make_equal(int *d, int n)
{
    int *t = malloc((size_t)n * sizeof *t), len, j;

    d[n - 1] = 0;
    for (j = 0; j < n; j++)
        t[j] = d[j];
    for (len = n; len > 2; len--)
        for (j = 0; j + 1 < len; j++)
            t[j] = (t[j] + t[j + 1]) % 10;
    d[n - 1] = (t[0] - t[1] + 10) % 10;
    free(t);
}

void gen(int small)
{
    static const char *const sets[] = {
        "0123456789", "0123456789", "0123456789", "09", "19", "05", "789",
        "2468",
    };
    int n = small ? (int)rnd(3, 10) : 100, i, shape, make;
    int *d = malloc((size_t)n * sizeof *d);
    char *s = malloc((size_t)n + 1);
    const char *set;

    if (small) {
        set = sets[rnd(0, 7)];
        for (i = 0; i < n; i++)
            d[i] = rnd_char(set) - '0';
        make = rnd_chance(50);
    } else {
        shape = (int)(gen_seed() % 4);
        set = shape == 2 ? "99999998" : "0123456789";
        make = shape == 0 || shape == 2;
        for (i = 0; i < n; i++)
            d[i] = rnd_char(set) - '0';
        if (shape == 3)
            for (i = 1; i < n; i++)
                d[i] = d[0];
    }
    if (make)
        make_equal(d, n);
    for (i = 0; i < n; i++)
        s[i] = (char)('0' + d[i]);
    s[n] = '\0';
    put_string(s);
    free(s);
    free(d);
}
