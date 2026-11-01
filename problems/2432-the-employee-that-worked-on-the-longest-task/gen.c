/* Ids drawn at random, never the same twice in a row. The seed picks the
 * task times. Small cases (2 to 5 employees, 1 to 8 tasks): times of 1 to
 * 4, so ties are common; all times equal; or short times with one long
 * task first, last or anywhere. Full-size cases (up to 500 employees):
 * 500 tasks of one minute each, ending at 1 to 500, so all of them tie; a
 * random set of finishing minutes from 1 to 500; or one minute per task
 * with a long task first or last that takes the log to minute 500. */
static int asc(const void *x, const void *y)
{
    int a = *(const int *)x, b = *(const int *)y;

    return (a > b) - (a < b);
}

void gen(int small)
{
    static int ends[500], minutes[500], rows[1000];
    int n, m, shape = (int)rnd(0, 3), d = 1, at = -1, big = 1, i, id;

    if (small) {
        n = (int)rnd(2, 5);
        m = (int)rnd(1, 8);
        if (shape == 1)
            d = (int)rnd(1, 5);
        if (shape >= 2) {
            i = (int)rnd(0, 2);
            at = i == 0 ? 0 : i == 1 ? m - 1 : (int)rnd(0, m - 1);
            big = (int)rnd(2, 9);
        }
        for (i = 0; i < m; i++) {
            if (shape == 0)
                d = (int)rnd(1, 4);
            else if (shape >= 2)
                d = i == at ? big : (int)rnd(1, 2);
            ends[i] = (i == 0 ? 0 : ends[i - 1]) + d;
        }
    } else {
        n = rnd_chance(50) ? 500 : (int)rnd(2, 500);
        if (shape == 0) {
            m = 500;
            for (i = 0; i < m; i++)
                ends[i] = i + 1;
        } else if (shape == 1) {
            m = (int)rnd(1, 500);
            for (i = 0; i < 500; i++)
                minutes[i] = i + 1;
            rnd_shuffle(minutes, 500);
            memcpy(ends, minutes, (size_t)m * sizeof *ends);
            qsort(ends, (size_t)m, sizeof *ends, asc);
        } else {
            m = (int)rnd(1, 499);
            at = shape == 2 ? 0 : m - 1;
            for (i = 0; i < m; i++)
                ends[i] = (i == 0 ? 0 : ends[i - 1]) +
                          (i == at ? 500 - (m - 1) : 1);
        }
    }
    for (i = 0; i < m; i++) {
        id = (int)rnd(0, i == 0 ? n - 1 : n - 2);
        if (i > 0 && id >= rows[2 * (i - 1)])
            id++;
        rows[2 * i] = id;
        rows[2 * i + 1] = ends[i];
    }
    put_int(n);
    put_int_grid(rows, m, 2);
}
