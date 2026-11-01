/* One n. Small cases are spread over the whole range, since brute.c is
 * quick for any n: often below 40, where ties are common, sometimes in
 * the hundreds, and sometimes anywhere up to 10^4. Full-size cases are
 * near the top: 9999, 10^4, or above 9000. */
void gen(int small)
{
    int shape = (int)rnd(0, 2);

    if (small) {
        if (shape == 0)
            put_int(rnd(1, 40));
        else if (shape == 1)
            put_int(rnd(1, 1000));
        else
            put_int(rnd(1, 10000));
    } else {
        if (shape == 0)
            put_int(10000);
        else if (shape == 1)
            put_int(9999);
        else
            put_int(rnd(9000, 10000));
    }
}
