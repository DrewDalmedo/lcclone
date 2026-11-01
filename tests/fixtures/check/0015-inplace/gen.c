/* Up to 6 small values, and a val that is often among them. */
void gen(int small)
{
    int n = (int)rnd(0, small ? 6 : 50), i, a[50];

    for (i = 0; i < n; i++)
        a[i] = (int)rnd(0, 4);
    put_ints(a, n);
    put_int(rnd(0, 4));
}
