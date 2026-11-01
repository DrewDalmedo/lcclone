/* Random cells with many zeros. Usually the cell where the sums on its two
 * sides come closest is emptied, and the sides are then nudged to differ
 * by 0, 1 or 2, since those are the launches that can clear the row. A
 * full-size case is often half empty cells and half walls of 100, which
 * makes a step-by-step simulation slow. */

/* Moves *need units into (add) or out of the cells a[lo..hi], starting at
 * a random cell and wrapping around, keeping each cell from 0 to top;
 * when walls_only is set, empty cells are left alone. */
static void nudge(int *a, int lo, int hi, int *need, int top, int add,
                  int walls_only)
{
    int len = hi - lo + 1, first, j, i, room;

    if (len <= 0)
        return;
    first = (int)rnd(0, len - 1);
    for (j = 0; j < len && *need > 0; j++) {
        i = lo + (first + j) % len;
        if (walls_only && a[i] == 0)
            continue;
        room = add ? top - a[i] : a[i];
        if (room > *need)
            room = *need;
        a[i] += add ? room : -room;
        *need -= room;
    }
}

void gen(int small)
{
    int n = small ? (int)rnd(1, 8) : 100;
    int top = small ? 5 : 100;
    int empty = (int)rnd(20, 60), slow = !small && rnd_chance(40);
    int *nums = malloc((size_t)n * sizeof *nums);
    int i, s, left, right, total = 0, gap, best = -1, delta, need, r;

    for (i = 0; i < n; i++) {
        if (rnd_chance(slow ? 50 : empty))
            nums[i] = 0;
        else if (slow && rnd_chance(80))
            nums[i] = top;
        else
            nums[i] = (int)rnd(1, top);
        total += nums[i];
    }
    r = (int)rnd(0, 9);
    if (r == 9) {
        /* left as drawn, apart from one empty cell */
        s = (int)rnd(0, n - 1);
        nums[s] = 0;
    } else {
        /* empty the cell whose sides come closest to equal */
        s = 0;
        for (i = 0, left = 0; i < n; left += nums[i], i++) {
            gap = left - (total - left - nums[i]);
            if (gap < 0)
                gap = -gap;
            if (best < 0 || gap < best) {
                best = gap;
                s = i;
            }
        }
        nums[s] = 0;
        left = right = 0;
        for (i = 0; i < s; i++)
            left += nums[i];
        for (i = s + 1; i < n; i++)
            right += nums[i];
        delta = r < 4 ? 0 : r < 6 ? 1 : r < 8 ? -1 : 2;
        need = left - right - delta;
        if (need > 0) {
            /* the left side is too heavy */
            nudge(nums, s + 1, n - 1, &need, top, 1, 1);
            nudge(nums, 0, s - 1, &need, top, 0, 0);
            nudge(nums, s + 1, n - 1, &need, top, 1, 0);
        } else if (need < 0) {
            need = -need;
            nudge(nums, 0, s - 1, &need, top, 1, 1);
            nudge(nums, s + 1, n - 1, &need, top, 0, 0);
            nudge(nums, 0, s - 1, &need, top, 1, 0);
        }
    }
    put_ints(nums, n);
    free(nums);
}
