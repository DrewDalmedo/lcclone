/* Small cases: 4 to 8 values, either all from a narrow range or values
 * 1 and 2 followed by three from 3 to 6, so that sums match and values
 * repeat often. Full size: 50 values, in a shape the seed picks:
 * any values; values up to 12 (many matches); small values first and their
 * possible sums after (tens of thousands of matches); or small values
 * first and large ones last. */
void gen(int small)
{
    int nums[50];
    int n = small ? (int)rnd(4, 8) : 50, shape, top, k, i;

    if (small) {
        shape = rnd_chance(60);
        top = (int)rnd(3, 10);
        for (i = 0; i < n; i++) {
            if (shape)
                nums[i] = i < n - 3 ? (int)rnd(1, 2) : (int)rnd(3, 6);
            else
                nums[i] = (int)rnd(1, top);
        }
    } else {
        shape = (int)rnd(0, 3);
        k = (int)rnd(30, 44);
        for (i = 0; i < n; i++) {
            if (shape == 0)
                nums[i] = (int)rnd(1, 100);
            else if (shape == 1)
                nums[i] = (int)rnd(1, 12);
            else if (shape == 2)
                nums[i] = i < k ? (int)rnd(1, 2) : (int)rnd(3, 6);
            else
                nums[i] = i < 40 ? (int)rnd(1, 33) : (int)rnd(50, 100);
        }
    }
    put_ints(nums, n);
}
