/* A majority value with at least floor(n / 2) + 1 copies, often exactly
 * that many, and the other elements all equal to one rival, all different
 * (only nearly so in full-size cases), or random, never equal to the
 * majority. Small cases use the values -4 to 4, or a quarter of the time
 * the whole range, where -10^9 and 10^9 come up often. The seed picks
 * the order: shuffled, the majority as one block at the start or at the
 * end, or alternating with the other elements. Full-size cases have
 * 5 * 10^4 elements or one fewer. */
static int value(int wide)
{
    if (!wide)
        return (int)rnd(-4, 4);
    switch (rnd(0, 3)) {
    case 0:
        return -1000000000;
    case 1:
        return 1000000000;
    default:
        return (int)rnd(-1000000000, 1000000000);
    }
}

static int other(int wide, int maj)
{
    int v;

    do
        v = value(wide);
    while (v == maj);
    return v;
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 9) : 50000 - (int)rnd(0, 1);
    int need = n / 2 + 1;
    int m = rnd_chance(60) ? need : (int)rnd(need, n);
    int r = n - m, wide = small ? rnd_chance(25) : 1;
    int maj = value(wide), kind = (int)rnd(0, 2), order = (int)rnd(0, 3);
    int *rest = malloc((size_t)(r + 1) * sizeof *rest);
    int *nums = malloc((size_t)n * sizeof *nums);
    int i, j, k, a, b, turn;

    for (i = 0; i < r; i++) {
        if (kind == 0)
            rest[i] = i == 0 ? other(wide, maj) : rest[0];
        else if (kind == 1 && small) {
            /* Different from each other: redraw a repeat (there are at
             * most 4 others, and 8 values to choose from). */
            do {
                rest[i] = other(wide, maj);
                for (j = 0; j < i && rest[j] != rest[i]; j++)
                    ;
            } while (j < i);
        } else if (kind == 1) {
            /* Uniform over the whole range: repeats are rare. */
            do
                rest[i] = (int)rnd(-1000000000, 1000000000);
            while (rest[i] == maj);
        } else
            rest[i] = other(wide, maj);
    }

    k = 0;
    if (order == 0 || order == 1) {
        for (i = 0; i < m; i++)
            nums[k++] = maj;
        for (i = 0; i < r; i++)
            nums[k++] = rest[i];
        if (order == 0)
            rnd_shuffle(nums, n);
    } else if (order == 2) {
        for (i = 0; i < r; i++)
            nums[k++] = rest[i];
        for (i = 0; i < m; i++)
            nums[k++] = maj;
    } else {
        /* Alternate, starting with either; the leftover copies of the
         * majority go at the end. */
        a = b = 0;
        turn = (int)rnd(0, 1);
        while (k < n) {
            if ((turn && a < m) || b == r)
                nums[k++] = maj, a++;
            else
                nums[k++] = rest[b++];
            turn = !turn;
        }
    }
    put_ints(nums, n);
    free(rest);
    free(nums);
}
