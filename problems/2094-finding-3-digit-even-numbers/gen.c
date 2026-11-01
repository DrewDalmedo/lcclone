/* Small cases are 3 to 8 cards drawn from one to four digit values, often
 * with 0 among them, so that repeated digits, leading zeros, inputs with
 * no even digit and inputs of only zeros come up often. Full-size ones
 * are 100 cards, in a shape the seed picks: digits from a few random
 * values, every value at least three times (all 450 numbers), odd digits
 * with a few copies of one even digit, or zeros with a few copies of one
 * other digit. */
void gen(int small)
{
    int digits[100], pool[4], n, k, i, v;

    if (small) {
        n = (int)rnd(3, 8);
        k = (int)rnd(1, 4);
        for (i = 0; i < k; i++)
            pool[i] = rnd_chance(30) ? 0 : (int)rnd(0, 9);
        for (i = 0; i < n; i++) {
            v = (int)rnd(0, k - 1);
            digits[i] = pool[v];
        }
    } else {
        n = 100;
        switch (gen_seed() % 4) {
        case 0: /* digits from two to four random values, maybe one 5 */
            k = (int)rnd(2, 4);
            for (i = 0; i < k; i++)
                pool[i] = (int)rnd(0, 9);
            for (i = 0; i < n; i++) {
                v = (int)rnd(0, k - 1);
                digits[i] = pool[v];
            }
            if (rnd_chance(50))
                digits[rnd(0, n - 1)] = 5;
            break;
        case 1: /* every value at least three times */
            for (i = 0; i < 30; i++)
                digits[i] = i % 10;
            for (i = 30; i < n; i++)
                digits[i] = (int)rnd(0, 9);
            break;
        case 2: /* odd digits and one to three copies of one even digit */
            v = 2 * (int)rnd(0, 4);
            k = (int)rnd(1, 3);
            for (i = 0; i < k; i++)
                digits[i] = v;
            for (i = k; i < n; i++)
                digits[i] = 2 * (int)rnd(0, 4) + 1;
            break;
        default: /* zeros and one to three copies of one other digit */
            v = (int)rnd(1, 9);
            k = (int)rnd(1, 3);
            for (i = 0; i < k; i++)
                digits[i] = v;
            for (i = k; i < n; i++)
                digits[i] = 0;
            break;
        }
        rnd_shuffle(digits, n);
    }
    put_ints(digits, n);
}
