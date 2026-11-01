/* Random values. Small cases: up to 12 elements from 1 to 9, so that n
 * is sometimes prime, sometimes a square and sometimes rich in divisors
 * (12). Full size: n is 48 (the most divisors), 50 (the longest), 36 (a
 * square with nine divisors), 47 (a prime) or random, and the values are
 * from 1 to 50, now and then all 50 for the largest total. */
void gen(int small)
{
    static const int sizes[] = {48, 50, 36, 47};
    int nums[50], n, top, shape, all_top, i;

    if (small) {
        n = (int)rnd(1, 12);
        top = 9;
        all_top = 0;
    } else {
        shape = (int)rnd(0, 4);
        n = shape < 4 ? sizes[shape] : (int)rnd(1, 50);
        top = 50;
        all_top = rnd_chance(20);
    }
    for (i = 0; i < n; i++)
        nums[i] = all_top ? top : (int)rnd(1, top);
    put_ints(nums, n);
}
