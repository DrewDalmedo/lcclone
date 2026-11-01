/* A circle of n numbers and a step count k. Small cases have n <= 8 and
 * numbers up to 9, so repeats are common; full-size ones have n = 100 and
 * numbers up to 100. A third of the cases take the most steps allowed,
 * k = n - 1 or -(n - 1), where every window wraps past the end. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? 9 : 100, k, i;
    int code[100];

    for (i = 0; i < n; i++)
        code[i] = (int)rnd(1, top);
    if (rnd_chance(33))
        k = rnd_chance(50) ? n - 1 : -(n - 1);
    else
        k = (int)rnd(-(n - 1), n - 1);
    put_ints(code, n);
    put_int(k);
}
