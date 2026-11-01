/* Records with distinct phone numbers and distinct seats. The phone and
 * seat digits are random, so they often look like ages above 60 while the
 * real age doesn't, and the other way round. The seed picks how ages are
 * drawn: often near 60 (58 to 63), so that 60 and 61 come up a lot; all
 * above 60; none above 60; or all in the 60s. Small cases have up to 8
 * records, full-size ones 100, which use up every seat from 00 to 99. */
static int pick_age(int shape)
{
    switch (shape) {
    case 1:
        return (int)rnd(61, 99);
    case 2:
        return (int)rnd(0, 60);
    case 3:
        return (int)rnd(60, 69);
    default:
        if (rnd_chance(50))
            return (int)rnd(58, 63);
        return (int)rnd(0, 99);
    }
}

void gen(int small)
{
    static char buf[100][16];
    char *rec[100];
    int seats[100];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 5), i, j, k, age, dup;

    if (shape > 3)
        shape = 0;
    for (i = 0; i < 100; i++)
        seats[i] = i;
    rnd_shuffle(seats, 100);
    for (i = 0; i < n; i++) {
        do {
            for (j = 0; j < 10; j++)
                buf[i][j] = rnd_char("0123456789");
            dup = 0;
            for (k = 0; k < i; k++)
                if (memcmp(buf[k], buf[i], 10) == 0)
                    dup = 1;
        } while (dup);
        buf[i][10] = rnd_char("MFO");
        age = pick_age(shape);
        buf[i][11] = (char)('0' + age / 10);
        buf[i][12] = (char)('0' + age % 10);
        buf[i][13] = (char)('0' + seats[i] / 10);
        buf[i][14] = (char)('0' + seats[i] % 10);
        buf[i][15] = '\0';
        rec[i] = buf[i];
    }
    put_strings(rec, n);
}
