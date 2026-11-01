/* A random pool of digits. The seed picks how the digits are drawn: each
 * from 0 to 9; from two or three values only, so digits repeat a lot;
 * all different (a shuffle of 0..9, cut short); about half of them 0; or
 * mostly odd. Small cases have 3 to 10 digits, most of them 3 to 5;
 * full-size ones have 10. */
void gen(int small)
{
    int n, shape, i, k, idx;
    int digits[10], shuffled[10], alpha[3];

    if (small)
        n = rnd_chance(60) ? (int)rnd(3, 5) : (int)rnd(6, 10);
    else
        n = 10;
    shape = (int)rnd(0, 4);
    k = (int)rnd(2, 3);
    for (i = 0; i < 3; i++)
        alpha[i] = (int)rnd(0, 9);
    for (i = 0; i < 10; i++)
        shuffled[i] = i;
    rnd_shuffle(shuffled, 10);
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            digits[i] = (int)rnd(0, 9);
        } else if (shape == 1) {
            idx = (int)rnd(0, k - 1);
            digits[i] = alpha[idx];
        } else if (shape == 2) {
            digits[i] = shuffled[i];
        } else if (shape == 3) {
            digits[i] = rnd_chance(50) ? 0 : (int)rnd(1, 9);
        } else {
            digits[i] = 2 * (int)rnd(0, 4);
            if (rnd_chance(80))
                digits[i]++;
        }
    }
    put_ints(digits, n);
}
