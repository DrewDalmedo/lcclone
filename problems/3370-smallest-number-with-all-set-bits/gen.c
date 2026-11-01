/* One n. The seed picks the shape: a power of two (just above an all-ones
 * number), an all-ones number itself, or any n. Small cases stay below 64;
 * full-size ones go up to 1000, where 512 is the slowest for counting up. */
void gen(int small)
{
    int top = small ? 63 : 1000, k, n;
    int shape = (int)rnd(0, 2);

    k = (int)rnd(0, small ? 5 : 9);
    if (shape == 0)
        n = 1 << k;
    else if (shape == 1)
        n = (2 << k) - 1;
    else
        n = (int)rnd(1, top);
    if (n > top)
        n = top;
    put_int(n);
}
