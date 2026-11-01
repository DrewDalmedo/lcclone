/* An array of numbers from 1 to 1000. Small cases have up to 8 numbers
 * from 1 to 12, each odd with a chance the seed picks, so runs of two
 * and three odd numbers come up often. Full-size ones have 1000 numbers
 * from the whole range, in a shape the seed picks: odd numbers in runs of
 * at most two, so there is no window and every position must be looked at;
 * the same with the only run of three at the very end; or each number odd
 * with a chance the seed picks. */
static int odd_value(int small)
{
    return 2 * (int)rnd(0, small ? 5 : 499) + 1;
}

static int even_value(int small)
{
    return 2 * (int)rnd(1, small ? 6 : 500);
}

void gen(int small)
{
    static const int chance[] = {20, 50, 70, 85, 100};
    int arr[1000];
    int n = small ? (int)rnd(1, 8) : 1000, shape, odds, run = 0, i;

    shape = small ? 2 : (int)(gen_seed() % 3);
    odds = chance[rnd(0, 4)];
    for (i = 0; i < n; i++) {
        if (shape == 2 ? rnd_chance(odds) : run < 2 && rnd_chance(70)) {
            arr[i] = odd_value(small);
            run++;
        } else {
            arr[i] = even_value(small);
            run = 0;
        }
    }
    if (shape == 1) {
        arr[n - 4] = even_value(small);
        for (i = n - 3; i < n; i++)
            arr[i] = odd_value(small);
    }
    put_ints(arr, n);
}
