/* s[0] is always '1'. Small cases have 1 to 8 characters; the seed picks
 * one run of 1s then 0s (true), that shape with one more '1' at the very
 * end (false, when it fits), or random characters. Full-size cases have
 * 100 characters, in a shape the seed picks: all ones; ones then zeros;
 * one 1 and zeros up to a last '1' (the second run is as late as it can
 * be); ones, zeros, then a run of ones to the end; random; or 1010... */
void gen(int small)
{
    char s[101];
    int n = small ? (int)rnd(1, 8) : 100, i, k, shape;

    s[0] = '1';
    for (i = 1; i < n; i++)
        s[i] = '0';
    s[n] = '\0';
    if (small) {
        shape = (int)rnd(0, 5);
        k = (int)rnd(1, n);
        if (shape <= 1) {
            for (i = 0; i < k; i++)
                s[i] = '1';
        } else if (shape == 2 && n >= 3) {
            k = (int)rnd(1, n - 2);
            for (i = 0; i < k; i++)
                s[i] = '1';
            s[n - 1] = '1';
        } else {
            for (i = 1; i < n; i++)
                s[i] = rnd_char("01");
        }
    } else {
        shape = (int)(gen_seed() % 6);
        if (shape == 0) {
            for (i = 0; i < n; i++)
                s[i] = '1';
        } else if (shape == 1) {
            k = (int)rnd(1, n - 1);
            for (i = 0; i < k; i++)
                s[i] = '1';
        } else if (shape == 2) {
            s[n - 1] = '1';
        } else if (shape == 3) {
            k = (int)rnd(1, n / 2);
            for (i = 0; i < k; i++)
                s[i] = '1';
            k = (int)rnd(n / 2 + 1, n - 1);
            for (i = k; i < n; i++)
                s[i] = '1';
        } else if (shape == 4) {
            for (i = 1; i < n; i++)
                s[i] = rnd_char("01");
        } else {
            for (i = 0; i < n; i++)
                s[i] = i % 2 == 0 ? '1' : '0';
        }
    }
    put_string(s);
}
