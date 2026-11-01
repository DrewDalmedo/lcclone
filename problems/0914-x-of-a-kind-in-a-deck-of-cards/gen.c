/* Small cases: most are decks of 1 to 3 different numbers (from 0, 1, 2 and
 * 9999), each seen X, 2X or 3X times for one X from 1 to 3, and in about
 * 40% of them one number's count is one more or one less, which usually
 * spoils the deal; the rest are 1 to 8 cards from 0, 1 and 2.
 * Full-size cases fill the deck up to 10^4 cards, and the seed picks:
 *   - one number on every card;
 *   - every card different;
 *   - counts that are all multiples of one X (2 to 12, 50, 97, 1000 or
 *     the prime 4999), with as many numbers as fit;
 *   - the same, then the last number's count one more or one less;
 *   - every number twice, or every number twice but the last one, which
 *     is seen three times;
 *   - random cards from a few numbers.
 * The deck is then shuffled, kept grouped, or kept grouped and reversed
 * (so that a spoiling count comes first). */
static int *deck, n;

static void add(int value, int copies)
{
    while (copies-- > 0)
        deck[n++] = value;
}

void gen(int small)
{
    static const int xs[] = {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 50, 97,
                             1000, 4999};
    int pool[4] = {0, 1, 2, 9999};
    int shape = (int)rnd(0, 5), order = (int)rnd(0, 2), d, x, m, i, c;
    int off, delta, size, top, t;
    int *vals;

    deck = malloc(10000 * sizeof *deck);
    n = 0;
    if (small) {
        if (shape == 0) {
            size = (int)rnd(1, 8);
            for (i = 0; i < size; i++)
                add((int)rnd(0, 2), 1);
        } else {
            rnd_shuffle(pool, 4);
            d = (int)rnd(1, 3);
            x = (int)rnd(1, 3);
            off = rnd_chance(40) ? (int)rnd(0, d - 1) : -1;
            delta = rnd_chance(50) ? 1 : -1;
            for (i = 0; i < d; i++) {
                m = (int)rnd(1, 3);
                c = x * m;
                if (i == off && (delta > 0 || c > 1))
                    c += delta;
                add(pool[i], c);
            }
        }
        rnd_shuffle(deck, n);
    } else {
        vals = malloc(10000 * sizeof *vals);
        for (i = 0; i < 10000; i++)
            vals[i] = i;
        rnd_shuffle(vals, 10000);
        size = rnd_chance(70) ? 10000 : (int)rnd(1, 10000);
        if (shape == 0) {
            add(vals[0], size);
        } else if (shape == 1) {
            for (i = 0; i < size; i++)
                add(vals[i], 1);
        } else if (shape <= 3) {
            /* leave room for one more card */
            x = xs[rnd(0, 14)];
            for (i = 0; n + x <= 9999; i++) {
                top = (9999 - n) / x;
                m = (int)rnd(1, top < 3 ? top : 3);
                add(vals[i], x * m);
            }
            if (shape == 3) {
                if (rnd_chance(50))
                    add(vals[i - 1], 1);
                else
                    n--;
            }
        } else if (shape == 4) {
            for (i = 0; 2 * i + 2 <= 10000; i++)
                add(vals[i], 2);
            if (rnd_chance(50)) {
                n -= 2;
                add(vals[i - 2], 1);
            }
        } else {
            top = (int)rnd(0, 20);
            for (i = 0; i < size; i++)
                add(vals[rnd(0, top)], 1);
        }
        free(vals);
        if (order == 0) {
            rnd_shuffle(deck, n);
        } else if (order == 1) {
            for (i = 0; i < n / 2; i++) {
                t = deck[i];
                deck[i] = deck[n - 1 - i];
                deck[n - 1 - i] = t;
            }
        }
    }
    put_ints(deck, n);
    free(deck);
}
