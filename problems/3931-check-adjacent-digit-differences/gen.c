/* Digit strings shaped as walks: smooth ones (every step at most 2), the
 * same with one jump of 3 or more (often the first or the last move),
 * walks with steps of up to 3 (so 2s and 3s meet often), and digits drawn
 * freely. Small cases have 2 to 8 digits, full-size ones 100. */
static int walk(int v, int maxstep)
{
    int d = (int)rnd(-maxstep, maxstep);

    if (v + d < 0 || v + d > 9)
        d = -d;
    return v + d;
}

static int jump(int v)
{
    int top = v > 9 - v ? v : 9 - v;
    int d = rnd_chance(60) ? 3 : (int)rnd(3, top);
    int up;

    if (v + d > 9)
        up = 0;
    else if (v - d < 0)
        up = 1;
    else
        up = rnd_chance(50);
    return up ? v + d : v - d;
}

void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 100;
    int shape = (int)rnd(0, 5);
    int bad = -1, where, i, v;
    char s[101];

    if (shape == 2 || shape == 3) {
        where = (int)rnd(0, 3);
        if (where == 0)
            bad = 1;
        else if (where == 1)
            bad = n - 1;
        else
            bad = (int)rnd(1, n - 1);
    }
    v = (int)rnd(0, 9);
    s[0] = (char)('0' + v);
    for (i = 1; i < n; i++) {
        if (shape == 5)
            v = (int)rnd(0, 9);
        else if (shape == 4)
            v = walk(v, 3);
        else if (i == bad)
            v = jump(v);
        else
            v = walk(v, 2);
        s[i] = (char)('0' + v);
    }
    s[n] = '\0';
    put_string(s);
}
