/* Random lists of labels. Small cases draw a few labels from a pool of up
 * to four, which often includes 1 and 100, so that labels repeat with
 * every parity. Full-size cases have 100 entries, chosen by the seed: any
 * labels, all 100 labels once each in a random order (every bulb ends on),
 * or 50 random labels each entered twice, shuffled (every bulb ends off). */
void gen(int small)
{
    int bulbs[100], pool[4];
    int n, i, k, shape;

    if (small) {
        n = (int)rnd(1, 8);
        k = (int)rnd(1, 4);
        for (i = 0; i < k; i++) {
            shape = (int)rnd(0, 3);
            pool[i] = shape == 0 ? 1 : shape == 1 ? 100 : (int)rnd(1, 100);
        }
        for (i = 0; i < n; i++)
            bulbs[i] = pool[rnd(0, k - 1)];
    } else {
        n = 100;
        shape = (int)rnd(0, 2);
        for (i = 0; i < n; i++) {
            if (shape == 0)
                bulbs[i] = (int)rnd(1, 100);
            else if (shape == 1)
                bulbs[i] = i + 1;
            else
                bulbs[i] = (int)rnd(1, 100);
        }
        if (shape == 2)
            for (i = 0; i < 50; i++)
                bulbs[50 + i] = bulbs[i];
        rnd_shuffle(bulbs, n);
    }
    put_ints(bulbs, n);
}
