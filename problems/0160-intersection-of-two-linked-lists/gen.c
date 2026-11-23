/* Two lists: a nodes of A's own, b of B's own, then s shared. At full size:
 * seed 1 is two separate lists of 3 * 10^4, 2 shares only the last node, 3
 * is one list given twice, 4 has B as just A's last node, 5 is two separate
 * lists of equal values, and 6 joins after 10 nodes of A and 20000 of B,
 * with all values equal. */
void gen(int small)
{
    int a, b, s, m, n, i, top = small ? 3 : 100000;
    int shape = small ? -1 : (int)((gen_seed() - 1) % 6);
    int *la, *lb;

    if (small) {
        a = (int)rnd(0, 3);
        b = (int)rnd(0, 3);
        s = (int)rnd(0, 3);
        if (s == 0) {
            if (a == 0)
                a = 1;
            if (b == 0)
                b = 1;
        }
    } else {
        static const int sizes[6][3] = {
            {30000, 30000, 0}, {29999, 29999, 1}, {0, 0, 30000},
            {29999, 0, 1},     {30000, 30000, 0}, {10, 20000, 10000},
        };

        a = sizes[shape][0];
        b = sizes[shape][1];
        s = sizes[shape][2];
    }
    m = a + s;
    n = b + s;
    la = malloc(((size_t)m + 1) * sizeof *la);
    lb = malloc(((size_t)n + 1) * sizeof *lb);
    for (i = 0; i < m; i++)
        la[i] = shape == 4 ? 1 : shape == 5 ? 100000 : (int)rnd(1, top);
    for (i = 0; i < b; i++)
        lb[i] = shape == 4 ? 1 : shape == 5 ? 100000 : (int)rnd(1, top);
    for (i = 0; i < s; i++)
        lb[b + i] = la[a + i];
    put_list(la, m);
    put_list(lb, n);
    put_int(a);
    put_int(b);
    free(la);
    free(lb);
}
