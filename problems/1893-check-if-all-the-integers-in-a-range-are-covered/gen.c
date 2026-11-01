/* Posts go from 1 to top: top is 1 to 12 for small cases, with 1 to 6
 * stretches, and 50 for full-size ones, with 50 stretches (or, now and
 * then, fewer). left..right is random, or the whole fence 1..50 for some
 * full-size cases. The seed picks the stretches:
 *   - short random stretches;
 *   - a chain of short stretches from somewhere at or below left to
 *     somewhere at or above right, each one touching or overlapping the
 *     one before by up to 2 posts, then short random stretches in the
 *     slots left over: every post from left to right is painted;
 *   - the same, then one post from left to right is cut out of every
 *     stretch that includes it (a one-post stretch moves elsewhere), so
 *     exactly that post is missed.
 * The stretches are then listed in a random order. */
static int top, s[50], e[50];

static void random_stretch(int k, int longest)
{
    int len = (int)rnd(0, longest < top - 1 ? longest : top - 1);

    s[k] = (int)rnd(1, top - len);
    e[k] = s[k] + len;
}

void gen(int small)
{
    int m, left, right, shape, longest, lo, hi, pos, len, back, x, y, k, i;
    int order[50], flat[100];

    top = small ? (int)rnd(1, 12) : 50;
    m = small ? (int)rnd(1, 6) : (rnd_chance(80) ? 50 : (int)rnd(1, 49));
    left = (int)rnd(1, top);
    right = (int)rnd(left, top);
    if (!small && rnd_chance(30)) {
        left = 1;
        right = 50;
    }
    longest = small ? 3 : 5;
    shape = (int)rnd(0, 2);
    k = 0;
    if (shape > 0) {
        lo = (int)rnd(1, left);
        hi = (int)rnd(right, top);
        for (pos = lo; pos <= hi && k < m; k++) {
            len = (int)rnd(1, longest);
            back = (int)rnd(0, 2);
            s[k] = pos - back < 1 ? 1 : pos - back;
            e[k] = pos + len - 1 > hi ? hi : pos + len - 1;
            pos = e[k] + 1;
        }
        if (pos <= hi)
            e[k - 1] = hi;
    }
    for (; k < m; k++)
        random_stretch(k, longest);
    if (shape == 2 && top > 1) {
        x = (int)rnd(left, right);
        for (i = 0; i < m; i++) {
            if (s[i] > x || e[i] < x)
                continue;
            if (s[i] == e[i]) {
                y = (int)rnd(1, top - 1);
                if (y >= x)
                    y++;
                s[i] = e[i] = y;
            } else if (s[i] == x) {
                s[i] = x + 1;
            } else if (e[i] == x) {
                e[i] = x - 1;
            } else if (rnd_chance(50)) {
                e[i] = x - 1;
            } else {
                s[i] = x + 1;
            }
        }
    }
    for (i = 0; i < m; i++)
        order[i] = i;
    rnd_shuffle(order, m);
    for (i = 0; i < m; i++) {
        flat[2 * i] = s[order[i]];
        flat[2 * i + 1] = e[order[i]];
    }
    put_int_grid(flat, m, 2);
    put_int(left);
    put_int(right);
}
