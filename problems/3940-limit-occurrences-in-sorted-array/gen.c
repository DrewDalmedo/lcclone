/* nums is made as blocks of equal values with random lengths, the values
 * of the blocks distinct and increasing. Block lengths are drawn around k,
 * so blocks shorter than, equal to and longer than k come up together,
 * often several long ones in a row. Small cases have up to 8 elements with
 * values from a few small ones (now and then up to 100), and k mostly 1 to
 * 3. Full-size cases have 100 elements; the seed picks the shape: k from 1
 * to 3 with blocks up to 2k + 2 long, a larger k, k at least as long as
 * every block, a single block, or k = 1 with blocks of 1 or 2. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, nums[100], len[100], pool[100];
    int k, shape, top, runs = 0, total = 0, pick, i, j, w = 0, v;

    shape = small ? (int)rnd(0, 3) : (int)(gen_seed() % 5);
    if (small)
        k = rnd_chance(25) ? (int)rnd(1, n) : (int)rnd(1, n < 3 ? n : 3);
    else if (shape == 0)
        k = (int)rnd(1, 3);
    else if (shape == 1)
        k = (int)rnd(4, 20);
    else if (shape == 2)
        k = (int)rnd(30, n);
    else if (shape == 3)
        k = (int)rnd(1, n);
    else
        k = 1;

    if (small && shape == 3)
        top = n;
    else if (small)
        top = k + 2;
    else if (shape == 0)
        top = 2 * k + 2;
    else if (shape == 1)
        top = 2 * k;
    else if (shape == 2)
        top = 30;
    else if (shape == 3)
        top = n;
    else
        top = 2;
    while (total < n) {
        pick = !small && shape == 3 ? n : (int)rnd(1, top);
        if (pick > n - total)
            pick = n - total;
        len[runs++] = pick;
        total += pick;
    }

    /* runs distinct values from 1 to P, in increasing order */
    if (small)
        v = rnd_chance(20) ? 100 : runs + 2;
    else
        v = 100;
    for (i = 0; i < v; i++)
        pool[i] = i + 1;
    rnd_shuffle(pool, v);
    for (i = 1; i < runs; i++)
        for (j = i; j > 0 && pool[j - 1] > pool[j]; j--) {
            pick = pool[j];
            pool[j] = pool[j - 1];
            pool[j - 1] = pick;
        }
    for (i = 0; i < runs; i++)
        for (j = 0; j < len[i]; j++)
            nums[w++] = pool[i];
    put_ints(nums, n);
    put_int(k);
}
