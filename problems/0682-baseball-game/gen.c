/* A list of commands, simulated in 64 bits while it is made, so that every
 * "+", "D" and "C" finds enough lines on the tape and the absolute values
 * on the tape never add up to more than 2^31 - 1. A command that would
 * break that bound becomes a "C" instead (the tape is never blank then).
 *
 * Small cases: 1 to 9 commands, mostly integers from -9 to 9, now and then
 * a five-digit one. Full-size cases: 1000 commands, in a shape the seed
 * picks:
 *   0. a mix of all four commands;
 *   1. integers only, 1000 numbers on the tape at the end;
 *   2. large integers and long runs of "D" and "+", which keep reaching
 *      the bound (numbers past 10^9);
 *   3. many "C"s, so that the tape keeps emptying and filling again. */
#define BOUND 2147483647LL

static const int weights[5][4] = {
    /* integer, "+", "D", "C" */
    {40, 20, 20, 20}, /* small */
    {40, 20, 20, 20},
    {100, 0, 0, 0},
    {10, 30, 45, 15},
    {45, 10, 10, 35},
};

static long long magnitude(long long v)
{
    return v < 0 ? -v : v;
}

static long long integer(int small, int shape)
{
    long long v, sign;

    if (small) {
        if (rnd_chance(15))
            return rnd(-30000, 30000);
        return rnd(-9, 9);
    }
    if (rnd_chance(4)) {
        sign = rnd_chance(50) ? 1 : -1;
        return sign * 30000;
    }
    if (shape == 2) {
        v = rnd(15000, 30000);
        sign = rnd_chance(50) ? 1 : -1;
        return sign * v;
    }
    return rnd(-30000, 30000);
}

void gen(int small)
{
    static char text[1000][12];
    static char *ops[1000];
    static long long tape[1000];
    int shape = small ? -1 : (int)rnd(0, 3);
    const int *w = weights[shape + 1];
    int n = small ? (int)rnd(1, 9) : 1000, top = 0, i, r, kind;
    long long total = 0, v = 0;

    for (i = 0; i < n; i++) {
        r = (int)rnd(0, 99);
        kind = r < w[0] ? 0 : r < w[0] + w[1] ? 1 : r < w[0] + w[1] + w[2] ? 2 : 3;
        if ((kind == 1 && top < 2) || (kind >= 2 && top < 1))
            kind = 0;
        if (kind == 0)
            v = integer(small, shape);
        else if (kind == 1)
            v = tape[top - 1] + tape[top - 2];
        else if (kind == 2)
            v = 2 * tape[top - 1];
        if (kind != 3 && total + magnitude(v) > BOUND)
            kind = 3;
        if (kind == 3) {
            top--;
            total -= magnitude(tape[top]);
            strcpy(text[i], "C");
        } else {
            tape[top++] = v;
            total += magnitude(v);
            if (kind == 0)
                snprintf(text[i], sizeof text[i], "%d", (int)v);
            else
                strcpy(text[i], kind == 1 ? "+" : "D");
        }
        ops[i] = text[i];
    }
    put_strings(ops, n);
}
