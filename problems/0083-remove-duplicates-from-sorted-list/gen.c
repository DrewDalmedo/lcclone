/* A sorted list. At full size: seed 1 is 300 random values, 2 is 300 equal
 * values, 3 every value once (201 nodes), 4 runs of exactly two, and 5 one
 * low value and a run of 299 at the top. */
void gen(int small)
{
    int a[300], count[201] = {0}, n = 300, i, v, k = 0;
    int shape = small ? -1 : (int)((gen_seed() - 1) % 5);

    if (small) {
        n = (int)rnd(0, 8);
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, 4);
            count[v]++;
        }
        for (v = 0; v <= 4; v++)
            for (i = 0; i < count[v]; i++)
                a[k++] = v - 2;
    } else if (shape == 0) {
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, 200);
            count[v]++;
        }
        for (v = 0; v <= 200; v++)
            for (i = 0; i < count[v]; i++)
                a[k++] = v - 100;
    } else if (shape == 1) {
        for (i = 0; i < n; i++)
            a[i] = -100;
    } else if (shape == 2) {
        n = 201;
        for (i = 0; i < n; i++)
            a[i] = i - 100;
    } else if (shape == 3) {
        for (i = 0; i < n; i++)
            a[i] = -100 + i / 2;
    } else {
        a[0] = -100;
        for (i = 1; i < n; i++)
            a[i] = 100;
    }
    put_list(a, n);
}
