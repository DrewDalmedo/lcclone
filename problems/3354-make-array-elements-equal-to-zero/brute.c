/* Slow and obviously right: play out every launch, step by step, on a
 * fresh copy of nums, and check the copy once the ball has left. */
int countValidSelections(int *nums, int numsSize)
{
    int *cells = malloc((size_t)numsSize * sizeof *cells);
    int start, dir, cur, d, i, clear, count = 0;

    for (start = 0; start < numsSize; start++) {
        if (nums[start] != 0)
            continue;
        for (dir = -1; dir <= 1; dir += 2) {
            memcpy(cells, nums, (size_t)numsSize * sizeof *cells);
            cur = start;
            d = dir;
            while (cur >= 0 && cur < numsSize) {
                if (cells[cur] == 0) {
                    cur += d;
                } else {
                    cells[cur]--;
                    d = -d;
                    cur += d;
                }
            }
            clear = 1;
            for (i = 0; i < numsSize; i++)
                if (cells[i] != 0)
                    clear = 0;
            count += clear;
        }
    }
    free(cells);
    return count;
}
