/* Numbers from 1 to 99. The seed picks the shape:
 *   0: each number short (one digit) or long (two digits) at random;
 *   1: the short numbers add up to exactly the long ones (a tie);
 *   2: such a tie, then one number moved up or down by one within its kind;
 *   3: numbers of one kind only.
 * Small cases have 1 to 8 numbers, long ones from 10 to 20, so 9 and 10
 * come up often; full-size cases have 100 numbers, long ones up to 99. */

/* Fills a[0..cnt) with numbers from lo to hi that add up to sum, which
 * must lie between lo * cnt and hi * cnt. */
static void fill(int *a, int cnt, int lo, int hi, int sum)
{
    int i, rem = sum - lo * cnt;

    for (i = 0; i < cnt; i++)
        a[i] = lo;
    while (rem > 0) {
        i = (int)rnd(0, cnt - 1);
        if (a[i] < hi) {
            a[i]++;
            rem--;
        }
    }
}

void gen(int small)
{
    int len = small ? (int)rnd(1, 8) : 100;
    int toplong = small ? 20 : 99;
    int shape = (int)rnd(0, 3);
    int *a = malloc((size_t)len * sizeof *a);
    int i, m, cnt1, lo, hi, sum, kind, up;

    if (shape == 1 || shape == 2) {
        if (len < 3) {
            shape = 0;
        } else {
            /* m long numbers and cnt1 short ones with equal totals:
             * 10 * m <= 9 * cnt1 keeps a common total possible. */
            m = (int)rnd(1, 9 * len / 19);
            cnt1 = len - m;
            lo = 10 * m > cnt1 ? 10 * m : cnt1;
            hi = toplong * m < 9 * cnt1 ? toplong * m : 9 * cnt1;
            sum = (int)rnd(lo, hi);
            fill(a, m, 10, toplong, sum);
            fill(a + m, cnt1, 1, 9, sum);
            if (shape == 2) {
                i = (int)rnd(0, len - 1);
                up = (int)rnd(0, 1);
                if (a[i] == 1 || a[i] == 10)
                    up = 1;
                else if (a[i] == 9 || a[i] == 99)
                    up = 0;
                a[i] += up ? 1 : -1;
            }
        }
    }
    if (shape == 0) {
        for (i = 0; i < len; i++) {
            if (rnd_chance(50))
                a[i] = (int)rnd(1, 9);
            else
                a[i] = (int)rnd(10, toplong);
        }
    } else if (shape == 3) {
        kind = (int)rnd(0, 1);
        for (i = 0; i < len; i++)
            a[i] = kind ? (int)rnd(10, toplong) : (int)rnd(1, 9);
    }
    rnd_shuffle(a, len);
    put_ints(a, len);
    free(a);
}
