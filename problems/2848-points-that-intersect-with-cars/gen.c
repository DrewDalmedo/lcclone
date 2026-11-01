/* Markers go from 1 to top: 1 to 12 for small cases, with 1 to 6 cars,
 * and 100 for full-size ones, with 100 cars (or, now and then, fewer).
 * The seed picks the shape:
 *   - random cars, mostly short, so that overlaps, shared ends and gaps
 *     all happen;
 *   - a chain of cars from left to right, each one overlapping the last,
 *     sharing its end marker, starting right after it or leaving a gap of
 *     one or more markers; the cars left over are copies of chain cars or
 *     pieces of them, so the gaps stay;
 *   - one long car, then random cars and repeats of earlier ones;
 *   - full size only: every car the whole street, 1 to 100.
 * The cars are then listed in a random order. */
static int top, longest, s[100], e[100];

static void random_car(int k)
{
    int most = rnd_chance(10) ? top - 1 : longest, len;

    if (most > top - 1)
        most = top - 1;
    len = (int)rnd(0, most);
    s[k] = (int)rnd(1, top - len);
    e[k] = s[k] + len;
}

void gen(int small)
{
    int m, shape, pos, len, step, chain, j, k, i;
    int order[100], flat[200];

    top = small ? (int)rnd(1, 12) : 100;
    m = small ? (int)rnd(1, 6) : (rnd_chance(80) ? 100 : (int)rnd(1, 99));
    longest = small ? 3 : (int)rnd(0, 12);
    shape = (int)rnd(0, small ? 2 : 3);
    k = 0;
    if (shape == 1) {
        pos = (int)rnd(1, top);
        while (k < m && pos <= top) {
            len = (int)rnd(0, longest);
            s[k] = pos;
            e[k] = pos + len > top ? top : pos + len;
            step = (int)rnd(-1, 2);
            pos = e[k] + step;
            if (step == 2 && rnd_chance(30))
                pos += (int)rnd(1, small ? 3 : 20);
            if (pos < s[k])
                pos = s[k];
            k++;
        }
        chain = k;
        for (; k < m; k++) {
            j = (int)rnd(0, chain - 1);
            s[k] = (int)rnd(s[j], e[j]);
            e[k] = (int)rnd(s[k], e[j]);
        }
    } else if (shape == 2) {
        s[0] = (int)rnd(1, top);
        e[0] = (int)rnd(s[0], top);
        for (k = 1; k < m; k++) {
            if (rnd_chance(40)) {
                j = (int)rnd(0, k - 1);
                s[k] = s[j];
                e[k] = e[j];
            } else {
                random_car(k);
            }
        }
    } else if (shape == 3) {
        for (; k < m; k++) {
            s[k] = 1;
            e[k] = 100;
        }
    }
    for (; k < m; k++)
        random_car(k);
    for (i = 0; i < m; i++)
        order[i] = i;
    rnd_shuffle(order, m);
    for (i = 0; i < m; i++) {
        flat[2 * i] = s[order[i]];
        flat[2 * i + 1] = e[order[i]];
    }
    put_int_grid(flat, m, 2);
}
