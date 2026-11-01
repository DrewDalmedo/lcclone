/* A row of heights. Small cases have up to 8 pots with heights from 1 to
 * at most 5, so repeats and ties are common; full-size ones have 100 pots
 * with heights up to 100 (often from a narrow range, so heights repeat a
 * lot). The seed picks the shape: random, already tidy, reversed, tidy
 * with two pots swapped, tidy with one seedling moved to the far end, or
 * distinct heights 1 to n shuffled or reversed. */
static void sort_up(int *a, int n)
{
    int i, j, v;

    for (i = 1; i < n; i++) {
        v = a[i];
        for (j = i; j > 0 && a[j - 1] > v; j--)
            a[j] = a[j - 1];
        a[j] = v;
    }
}

static void reverse(int *a, int n)
{
    int i, t;

    for (i = 0; i < n / 2; i++) {
        t = a[i];
        a[i] = a[n - 1 - i];
        a[n - 1 - i] = t;
    }
}

void gen(int small)
{
    int heights[100];
    int n = small ? (int)rnd(1, 8) : 100;
    int shape = (int)rnd(0, 6);
    int top, i, j, t;

    if (small)
        top = (int)rnd(1, 5);
    else
        top = rnd_chance(40) ? 100 : (int)rnd(2, 100);
    for (i = 0; i < n; i++)
        heights[i] = (int)rnd(1, top);
    if (!small && top == 100 && n >= 2) {
        heights[0] = 100; /* both extremes present */
        heights[1] = 1;
        rnd_shuffle(heights, n);
    }
    switch (shape) {
    case 0: /* random */
        break;
    case 1: /* already tidy */
        sort_up(heights, n);
        break;
    case 2: /* reversed */
        sort_up(heights, n);
        reverse(heights, n);
        break;
    case 3: /* two pots swapped */
        sort_up(heights, n);
        i = (int)rnd(0, n - 1);
        j = (int)rnd(0, n - 1);
        t = heights[i];
        heights[i] = heights[j];
        heights[j] = t;
        break;
    case 4: /* the first seedling moved to the end, or the last to the front */
        sort_up(heights, n);
        if (rnd_chance(50)) {
            t = heights[0];
            for (i = 0; i + 1 < n; i++)
                heights[i] = heights[i + 1];
            heights[n - 1] = t;
        } else {
            t = heights[n - 1];
            for (i = n - 1; i > 0; i--)
                heights[i] = heights[i - 1];
            heights[0] = t;
        }
        break;
    default: /* distinct heights 1 to n, shuffled or reversed */
        for (i = 0; i < n; i++)
            heights[i] = i + 1;
        if (shape == 5)
            rnd_shuffle(heights, n);
        else
            reverse(heights, n);
    }
    put_ints(heights, n);
}
