/* Small cases: 1 to 8 values, mostly distinct from 1 to 100 so that any
 * wrong order shows; one in four from 1 to 3, with repeats. Full size:
 * 100 values, in a shape the seed picks: a shuffle of 1 to 100, random
 * values from 1 to 100, or 1 to 100 rising. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100, i, j, v, shape;
    int nums[100];

    if (small) {
        shape = rnd_chance(25);
        for (i = 0; i < n; i++) {
            if (shape) {
                nums[i] = (int)rnd(1, 3);
                continue;
            }
            do {
                v = (int)rnd(1, 100);
                for (j = 0; j < i && nums[j] != v; j++)
                    ;
            } while (j < i);
            nums[i] = v;
        }
    } else {
        shape = (int)rnd(0, 2);
        for (i = 0; i < n; i++)
            nums[i] = shape == 1 ? (int)rnd(1, 100) : i + 1;
        if (shape == 0)
            rnd_shuffle(nums, n);
    }
    put_ints(nums, n);
}
