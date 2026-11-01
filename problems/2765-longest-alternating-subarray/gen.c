/* Values from a narrow window [b, b + 3], so that steps of +1 and -1 come
 * up often. Most rows follow a seesaw that now and then breaks off and
 * restarts at a random height and phase (so some restarts begin on the
 * high value); others are random within the window, or use only b and
 * b + 2, which have no step of +1 (answer -1). The window is often at
 * either end of 1..10^4. Small cases: 2 to 8 values; full size: 99 or
 * 100 values, sometimes one seesaw over the whole row. */
void gen(int small)
{
    int n = small ? (int)rnd(2, 8) : (int)rnd(99, 100);
    int shape = (int)rnd(0, 9);
    int brk = small ? 30 : (int)rnd(1, 10);
    int where = (int)rnd(0, 3);
    int *nums = malloc((size_t)n * sizeof *nums);
    int b, low, off, i;

    b = where == 0 ? 1 : where == 1 ? 9997 : (int)rnd(1, 9997);
    low = b + (int)rnd(0, 2);
    off = shape == 2 ? 0 : (int)rnd(0, 1);
    /* shape 0: b and b + 2 only; 1: random in the window; 2: one seesaw
     * over the whole row; otherwise a seesaw that breaks at a chance of
     * brk in 100 */
    for (i = 0; i < n; i++) {
        if (shape == 0) {
            nums[i] = b + 2 * (int)rnd(0, 1);
        } else if (shape == 1) {
            nums[i] = b + (int)rnd(0, 3);
        } else {
            if (i > 0 && shape != 2 && rnd_chance(brk)) {
                low = b + (int)rnd(0, 2);
                off = (int)rnd(0, 1);
            }
            nums[i] = low + off % 2;
            off++;
        }
    }
    put_ints(nums, n);
    free(nums);
}
