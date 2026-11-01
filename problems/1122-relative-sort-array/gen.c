/* arr1 first, then arr2: some of arr1's distinct values, in a random
 * order. Small cases draw up to 8 values from 0..6 (and now and then
 * 1000), so values repeat. Full-size cases have 1000 values, in a shape
 * the seed picks:
 *   0: values from all of 0..1000, arr2 a random part of them;
 *   1: 1000 distinct values, all listed in arr2 (arr2.length is 1000);
 *   2: two values only, one of them filling nearly all of arr1, and arr2
 *      listing one or both;
 *   3: values from all of 0..1000, with 0 and 1000 always present and not
 *      listed, and arr2 listing many values from the largest down. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 1000;
    int shape = small ? -1 : (int)rnd(0, 3);
    int *arr1 = malloc((size_t)n * sizeof *arr1);
    int *arr2 = malloc(1001 * sizeof *arr2);
    char seen[1001] = {0};
    int d = 0, m, i, j, v, a, b, t;

    if (shape == 1) {
        v = (int)rnd(0, 1000);          /* the value left out */
        for (i = 0, j = 0; j <= 1000; j++)
            if (j != v)
                arr1[i++] = j;
        rnd_shuffle(arr1, n);
    } else if (shape == 2) {
        a = (int)rnd(0, 1000);
        do
            b = (int)rnd(0, 1000);
        while (b == a);
        t = (int)rnd(1, 3);             /* how many copies of b */
        for (i = 0; i < n; i++)
            arr1[i] = i < t ? b : a;
        rnd_shuffle(arr1, n);
    } else {
        for (i = 0; i < n; i++) {
            if (small) {
                v = rnd_chance(10) ? 1000 : (int)rnd(0, 6);
            } else {
                v = (int)rnd(0, 1000);
            }
            arr1[i] = v;
        }
        if (shape == 3) {
            arr1[0] = 0;
            arr1[1] = 1000;
            rnd_shuffle(arr1, n);
        }
    }
    for (i = 0; i < n; i++)
        if (!seen[arr1[i]]) {
            seen[arr1[i]] = 1;
            arr2[d++] = arr1[i];
        }
    if (shape == 1) {
        m = d;
    } else if (shape == 3) {
        /* every distinct value but 0 and 1000, from the largest down */
        m = 0;
        for (v = 999; v >= 1; v--)
            if (seen[v] && rnd_chance(90))
                arr2[m++] = v;
        if (m == 0) {
            for (v = 1; !seen[v]; v++)
                ;
            arr2[m++] = v;
        }
    } else {
        m = (int)rnd(1, d);
    }
    if (shape != 3)
        rnd_shuffle(arr2, d);
    put_ints(arr1, n);
    put_ints(arr2, m);
    free(arr1);
    free(arr2);
}
