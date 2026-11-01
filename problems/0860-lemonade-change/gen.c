/* Small cases are 1 to 8 bills: either each bill picked so that it can
 * still be changed (most of the time), or bills picked at random, most of
 * them 5s. Full-size cases are 10^5 bills, in a shape the seed picks:
 *   - every customer can be served, and a 20 can often be changed both
 *     ways (a 10 and a 5, or three 5s);
 *   - the same, but the box is run down near the end so that only the
 *     last customer can't be served (with 10^5 - 1, 10^5 - 2 or 10^5 - 3
 *     bills: with 10^5 that can't happen, since the box then holds
 *     5 * (10^5 - 1) in 5s, 10s and 20s, and every such box can change a
 *     10 and a 20);
 *   - one customer early on can't be served, and the ones after could
 *     all be served if the box started afresh there;
 *   - 5s and nothing else up to the last bill, a 10 (more 5s than a
 *     16-bit count holds);
 *   - a random length and random bills. */
static int fives, tens;

static int payable(int bill)
{
    if (bill == 5)
        return 1;
    if (bill == 10)
        return fives >= 1;
    return (tens >= 1 && fives >= 1) || fives >= 3;
}

static void take(int bill)
{
    if (bill == 5) {
        fives++;
    } else if (bill == 10) {
        fives--;
        tens++;
    } else if (tens >= 1 && fives >= 1) {
        tens--;
        fives--;
    } else {
        fives -= 3;
    }
}

/* a 5, 10 or 20 with chances w5, w10 and the rest in 100 */
static int pick(int w5, int w10)
{
    int r = (int)rnd(0, 99);

    return r < w5 ? 5 : r < w5 + w10 ? 10 : 20;
}

/* a bill that can be changed, picked with those chances where it can */
static int pick_payable(int w5, int w10)
{
    int bill = pick(w5, w10);

    return payable(bill) ? bill : 5;
}

/* bills[from..to): change can always be made, and there are often enough
 * 5s and a 10 to change a 20 both ways */
static void fill_payable(int *bills, int from, int to)
{
    int i;

    for (i = from; i < to; i++) {
        bills[i] = pick_payable(55, 20);
        take(bills[i]);
    }
}

/* Every customer can be served but the last one; 0 if this try didn't
 * manage it. Random payable bills come first. Once few enough customers
 * are left, the box is run down: a 20 when it holds a 5 and a 10, a 10
 * when it holds a 5 only, else a 5. That ends with at most one 5 and one
 * 10 in the box, and the last bill gets stuck unless it holds both, which
 * happens only when n is a multiple of 4. */
static int last_fails(int *bills, int n)
{
    int i;

    fives = tens = 0;
    for (i = 0; i < n - 1 && 3 * fives + 2 * tens + 8 < n - 1 - i; i++) {
        bills[i] = pick_payable(55, 20);
        take(bills[i]);
    }
    for (; i < n - 1; i++) {
        bills[i] = fives == 0 ? 5 : tens > 0 ? 20 : 10;
        take(bills[i]);
    }
    if (fives == 0)
        bills[n - 1] = rnd_chance(50) ? 10 : 20;
    else if (!payable(20))
        bills[n - 1] = 20;
    else
        return 0;
    return 1;
}

static int small_case(int *bills)
{
    int n = (int)rnd(1, 8), i;

    fives = tens = 0;
    if (rnd_chance(60)) {
        for (i = 0; i < n; i++) {
            bills[i] = rnd_chance(85) ? pick_payable(40, 35) : pick(40, 35);
            take(bills[i]);
        }
    } else {
        for (i = 0; i < n; i++)
            bills[i] = pick(55, 25);
    }
    return n;
}

static int large_case(int *bills)
{
    int n = 100000, shape = (int)(gen_seed() % 5), stop, i;

    fives = tens = 0;
    if (shape == 0) {
        fill_payable(bills, 0, n);
    } else if (shape == 1) {
        n -= (int)rnd(1, 3);
        for (i = 0; i < 100 && !last_fails(bills, n); i++)
            ;
        if (i == 100) {
            fives = tens = 0;
            fill_payable(bills, 0, n);
        }
    } else if (shape == 2) {
        stop = (int)rnd(1, 1000);
        fill_payable(bills, 0, stop);
        /* run the 5s down with 10s while a 20 could still be changed;
         * then a 10 or a 20 with no 5 in the box, or a 20 with neither a
         * 10 nor three 5s, gets stuck */
        while (fives > 0 && payable(20)) {
            bills[stop++] = 10;
            take(10);
        }
        bills[stop] = fives == 0 && rnd_chance(50) ? 10 : 20;
        /* from a fresh box, every later customer could be served */
        fives = tens = 0;
        fill_payable(bills, stop + 1, n);
    } else if (shape == 3) {
        for (i = 0; i < n - 1; i++)
            bills[i] = 5;
        bills[n - 1] = 10;
    } else {
        n = (int)rnd(1, n);
        for (i = 0; i < n; i++)
            bills[i] = pick(60, 25);
    }
    return n;
}

void gen(int small)
{
    int *bills = malloc(100000 * sizeof *bills);
    int n = small ? small_case(bills) : large_case(bills);

    put_ints(bills, n);
    free(bills);
}
