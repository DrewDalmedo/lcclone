/* Altitude changes along a trail. Small cases have up to 8 changes from
 * -5..5 (now and then from -100..100), leaning downhill, uphill or
 * neither, so that trails that never rise above the start, and trails
 * whose highest post is the last, come up often. Full-size cases
 * have 100 changes, in a shape the seed picks:
 *   0: changes from all of -100..100;
 *   1: mostly climbing, so the highest post is near the end, often the
 *      last one;
 *   2: mostly descending, so the answer is often 0;
 *   3: all +100 (the largest answer, 10000) or all -100 (lowest altitude
 *      -10000, answer 0). */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = small ? -1 : (int)(gen_seed() % 4);
    int top = small ? (rnd_chance(20) ? 100 : 5) : 100;
    int lean = small ? (int)rnd(0, 3) : 0;
    int sign = rnd_chance(50) ? 100 : -100;
    int *gain = malloc((size_t)n * sizeof *gain);
    int i;

    for (i = 0; i < n; i++) {
        if (shape == 1)
            gain[i] = (int)rnd(-60, 100);
        else if (shape == 2)
            gain[i] = (int)rnd(-100, 60);
        else if (shape == 3)
            gain[i] = sign;
        else if (lean == 1)
            gain[i] = (int)rnd(-top, top / 2);
        else if (lean == 2)
            gain[i] = (int)rnd(-top / 2, top);
        else
            gain[i] = (int)rnd(-top, top);
    }
    put_ints(gain, n);
    free(gain);
}
