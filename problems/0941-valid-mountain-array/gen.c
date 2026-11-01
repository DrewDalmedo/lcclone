/* Mountains are built from a top value V at index p, with distinct values
 * below V chosen for each side and sorted, rising before p and falling
 * after it. Small cases (up to 8 elements, values up to 10) are random
 * values from 0 to 4, a mountain, or a mountain with one change: a flat
 * step, one value replaced, or two neighbours swapped. Full-size cases have
 * 10^4 elements, in a shape the seed picks: a mountain with its top near
 * the end, or near the start; a mountain with one flat step near the end;
 * a mountain whose last step goes up again; or a strict rise with one flat
 * step near the end. */
static int vals[10000];

/* Writes k distinct values from [0, v - 1] to out in increasing order
 * (selection sampling: each value is taken with chance need / left). */
static void pick(int *out, int k, int v)
{
    int x, need = k, m = 0;

    for (x = 0; x < v && need > 0; x++)
        if (rnd(0, v - x - 1) < need) {
            out[m++] = x;
            need--;
        }
}

/* A mountain of n >= 3 elements with its top at index p. */
static void mountain(int *a, int n, int p, int top)
{
    int lo = p > n - 1 - p ? p : n - 1 - p, v, i;

    v = (int)rnd(lo, top);
    a[p] = v;
    pick(a, p, v);
    pick(vals, n - 1 - p, v);
    for (i = 0; i < n - 1 - p; i++)
        a[p + 1 + i] = vals[n - 2 - p - i];
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 10000;
    int *a = malloc((size_t)n * sizeof *a);
    int shape, kind, k, i, t, p;

    if (small) {
        shape = n < 3 ? 0 : (int)rnd(0, 2);
        if (shape == 0) {
            for (i = 0; i < n; i++)
                a[i] = (int)rnd(0, 4);
        } else {
            p = (int)rnd(1, n - 2);
            mountain(a, n, p, 10);
            if (shape == 2) {
                kind = (int)rnd(0, 2);
                k = (int)rnd(0, n - 2);
                if (kind == 0) {
                    a[k + 1] = a[k];
                } else if (kind == 1) {
                    t = (int)rnd(0, 10);
                    a[k] = t;
                } else {
                    t = a[k];
                    a[k] = a[k + 1];
                    a[k + 1] = t;
                }
            }
        }
    } else {
        shape = (int)(gen_seed() % 5);
        if (shape == 0)
            p = n - (int)rnd(2, 50);
        else if (shape == 1)
            p = (int)rnd(1, 49);
        else
            p = (int)rnd(1, n - 3);
        if (shape < 4) {
            mountain(a, n, p, 10000);
        } else {
            pick(a, n, 10001);
            k = n - (int)rnd(2, 50);
            a[k + 1] = a[k];
        }
        if (shape == 2) {
            k = n - (int)rnd(2, 50);
            a[k + 1] = a[k];
        } else if (shape == 3) {
            a[n - 1] = a[n - 2] + 1;
        }
    }
    put_ints(a, n);
    free(a);
}
