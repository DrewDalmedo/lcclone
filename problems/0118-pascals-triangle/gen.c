/* A number of rows: up to 12 for small cases (brute.c's recursion is
 * exponential), and for full-size ones 30, the largest, or 29, as the
 * seed picks. */
void gen(int small)
{
    if (small)
        put_int(rnd(1, 12));
    else
        put_int(30 - (long long)(gen_seed() % 2));
}
