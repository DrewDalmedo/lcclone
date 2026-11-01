/* Keys drawn from k letters of a shuffled alphabet (mostly 2 to 4 in
 * small cases, so letters repeat), and press times chosen first, then
 * added up into release times. The seed picks the times: random ones; all equal, so
 * every press ties; ones of 1 to 3, so ties are common; or all 1 but one
 * long press, which lands on the first press, the last or a random one.
 * Small cases have 2 to 8 presses with short times, though the long press
 * may be close to 10^9. Full-size cases have 1000 presses, with times up
 * to 10^6 (all equal reaches a last release of 10^9) and a long press that
 * takes the last release to exactly 10^9. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : 1000;
    int shape = (int)rnd(0, 3), k;
    int lt[26], *t = malloc((size_t)n * sizeof *t), i, d = 0, at = 0, big = 0;
    char *keys = malloc((size_t)n + 1);
    long long sum = 0;

    if (small)
        k = rnd_chance(8) ? 1 : (int)rnd(2, 4);
    else
        k = (int)rnd(1, 26);
    for (i = 0; i < 26; i++)
        lt[i] = i;
    rnd_shuffle(lt, 26);
    for (i = 0; i < n; i++)
        keys[i] = (char)('a' + lt[rnd(0, k - 1)]);
    keys[n] = '\0';
    if (shape == 1)
        d = small ? (int)rnd(1, 5) : (int)rnd(1, 1000000);
    if (shape == 3) {
        i = (int)rnd(0, 2);
        at = i == 0 ? 0 : i == 1 ? n - 1 : (int)rnd(0, n - 1);
        if (!small || rnd_chance(30))
            big = 1000000000 - (n - 1);
        else
            big = (int)rnd(2, 9);
    }
    for (i = 0; i < n; i++) {
        if (shape == 0)
            sum += small ? rnd(1, 5) : rnd(1, 999999);
        else if (shape == 1)
            sum += d;
        else if (shape == 2)
            sum += rnd(1, 3);
        else
            sum += i == at ? big : 1;
        t[i] = (int)sum;
    }
    put_ints(t, n);
    put_string(keys);
    free(t);
    free(keys);
}
