/* Two sorted lists. At full size, 50 nodes each: seed 1 random over the
 * whole range, 2 strictly alternating, 3 all of list1 below list2, 4 all
 * of list2 below list1, 5 every value 100, 6 list1 empty. */
static void sorted_draw(int *a, int n, int lo, int hi)
{
    int i, j;

    for (i = 0; i < n; i++) {
        int v = (int)rnd(lo, hi);

        for (j = i; j > 0 && a[j - 1] > v; j--)
            a[j] = a[j - 1];
        a[j] = v;
    }
}

void gen(int small)
{
    int a[50], b[50], n = 50, m = 50, i;
    int shape = small ? -1 : (int)((gen_seed() - 1) % 6);

    if (small) {
        n = (int)rnd(0, 5);
        m = (int)rnd(0, 5);
        sorted_draw(a, n, -3, 3);
        sorted_draw(b, m, -3, 3);
    } else if (shape == 0) {
        sorted_draw(a, n, -100, 100);
        sorted_draw(b, m, -100, 100);
        a[0] = -100;
        b[m - 1] = 100;
    } else if (shape == 1) {
        for (i = 0; i < 50; i++) {
            a[i] = -100 + 4 * i;
            b[i] = -98 + 4 * i;
        }
    } else if (shape == 2 || shape == 3) {
        int *low = shape == 2 ? a : b, *high = shape == 2 ? b : a;

        for (i = 0; i < 50; i++) {
            low[i] = -100 + i;
            high[i] = 51 + i;
        }
    } else if (shape == 4) {
        for (i = 0; i < 50; i++)
            a[i] = b[i] = 100;
    } else {
        n = 0;
        sorted_draw(b, m, -100, 100);
    }
    put_list(a, n);
    put_list(b, m);
}
