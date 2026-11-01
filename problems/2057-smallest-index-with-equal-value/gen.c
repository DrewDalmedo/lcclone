/* An array of digits. The seed picks the shape: random digits (so the
 * first agreeing index tends to come early); no agreeing index at all; or
 * none before a chosen index, which agrees, and some more after it, so the
 * first one can lie far from the front, past index 10. Small cases are up
 * to 30 long; full-size ones are 100, and often agree nowhere or only at
 * the last index, so a search has to look at every element. */
void gen(int small)
{
    int n = small ? (int)rnd(1, 30) : 100, i, d;
    int shape = (int)rnd(0, 3), first = -1;
    int nums[100];

    if (shape == 1)
        first = (int)rnd(0, n - 1);
    else if (shape == 3)
        first = n - 1;
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            nums[i] = (int)rnd(0, 9);
        } else if (i == first || (first >= 0 && i > first && rnd_chance(20))) {
            nums[i] = i % 10;
        } else {
            d = (int)rnd(1, 9);
            nums[i] = (i + d) % 10;
        }
    }
    put_ints(nums, n);
}
