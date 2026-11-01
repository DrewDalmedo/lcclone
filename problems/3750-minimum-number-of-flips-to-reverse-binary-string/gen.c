/* n's binary form is built from the outside in, one mirror pair at a time.
 * The seed picks how the two bits of a pair relate: at random, always equal
 * (a palindrome, answer 0), always different (the largest answer for that
 * length), or rarely different. Small cases have 1 to 10 bits; full-size
 * ones have 30 bits and stay at most 10^9, which also comes up itself. */
void gen(int small)
{
    int len = small ? (int)rnd(1, 10) : 30, shape = (int)rnd(0, 3);
    int bits[30], i, j, differ;
    long long n;

    if (!small && rnd_chance(5)) {
        put_int(1000000000);
        return;
    }
    do {
        for (i = 0, j = len - 1; i <= j; i++, j--) {
            bits[i] = i == 0 ? 1 : (int)rnd(0, 1);
            if (i == j)
                break;
            if (shape == 0)
                differ = (int)rnd(0, 1);
            else if (shape == 1)
                differ = 0;
            else if (shape == 2)
                differ = 1;
            else
                differ = rnd_chance(20);
            bits[j] = differ ? !bits[i] : bits[i];
        }
        n = 0;
        for (i = 0; i < len; i++)
            n = n * 2 + bits[i];
    } while (n > 1000000000);
    put_int(n);
}
