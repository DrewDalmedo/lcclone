/* One n. The seed picks its shape: random digits, random digits followed
 * by zeros, a palindrome, or, for full-size cases, 10^9 itself. Small
 * cases have 1 to 3 digits before the zeros; full-size ones 7 to 9 digits
 * in all, so that the distance can be large. */
void gen(int small)
{
    int shape = (int)rnd(0, 9), len, zeros = 0, i, d;
    long long n = 0;
    char digits[9];

    if (!small && shape == 9) {
        put_int(1000000000);
        return;
    }
    len = (int)rnd(small ? 1 : 7, small ? 3 : 9);
    if (shape == 4 || shape == 5) {
        zeros = (int)rnd(1, small ? 2 : 3);
        if (!small && len + zeros > 9)
            len = 9 - zeros;
    }
    for (i = 0; i < len; i++) {
        d = (int)rnd(i == 0 ? 1 : 0, 9);
        digits[i] = (char)d;
    }
    if (shape == 6 || shape == 7) {
        /* A palindrome: copy the first half onto the second. */
        for (i = 0; i < len / 2; i++)
            digits[len - 1 - i] = digits[i];
    }
    for (i = 0; i < len; i++)
        n = n * 10 + digits[i];
    for (i = 0; i < zeros; i++)
        n *= 10;
    put_int(n);
}
