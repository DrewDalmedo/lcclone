/* A strictly increasing array and a k. The values are a random set drawn
 * from lo..top (selection sampling, so they come out sorted). Small cases
 * have up to 8 values below 20 and k up to 12; the values start at 1 or a
 * little above, and sometimes are 1..n with no gaps. Full-size ones pick a
 * shape from the seed: nearly all of 1..1000 with k = 1000, so the answer
 * is near 2000 and a walk through the numbers is slowest; a random set and
 * k; or k landing on the last missing number before the end of arr. */
static void pick(int *arr, int n, int lo, int top)
{
    int v, need = n, i = 0;

    for (v = lo; v <= top && need > 0; v++)
        if (rnd(0, top - v) < need) {
            arr[i++] = v;
            need--;
        }
}

void gen(int small)
{
    int arr[1000];
    int n, lo, top, k, i, shape, gaps;

    if (small) {
        n = (int)rnd(1, 8);
        if (rnd_chance(15)) {
            for (i = 0; i < n; i++)
                arr[i] = i + 1;
        } else {
            lo = rnd_chance(40) ? 1 : (int)rnd(2, 8);
            top = lo - 1 + (int)rnd(n, 12);
            pick(arr, n, lo, top);
        }
        k = (int)rnd(1, 12);
    } else {
        shape = (int)rnd(0, 2);
        if (shape == 0) {
            n = (int)rnd(990, 1000);
            pick(arr, n, 1, 1000);
            k = 1000;
        } else {
            n = (int)rnd(1, 1000);
            pick(arr, n, 1, 1000);
            gaps = arr[n - 1] - n;
            if (shape == 1 || gaps < 1)
                k = (int)rnd(1, 1000);
            else
                k = gaps;
        }
    }
    put_ints(arr, n);
    put_int(k);
}
