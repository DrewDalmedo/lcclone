/* Slow and obviously right: search, breadth first, every arrangement of
 * arr that flips can reach, and see whether target is one of them. An
 * arrangement is kept as a number with 3 bits per position, holding the
 * rank of the value there among arr's distinct values, so this works for
 * at most 7 elements (gen.c's small cases keep to that). A value of target
 * that arr lacks can never turn up, since flips only move values. */
bool canBeEqual(int *target, int targetSize, int *arr, int arrSize)
{
    int n = arrSize, vals[7], d = 0, cur[7], i, j, l, r, k;
    int start = 0, goal = 0, head = 0, tail = 0, state, next;
    int *queue;
    unsigned char *seen;
    bool found = false;

    (void)targetSize;
    for (i = 0; i < n; i++) {
        for (j = 0; j < d && vals[j] != arr[i]; j++)
            ;
        if (j == d)
            vals[d++] = arr[i];
    }
    for (i = 0; i < n; i++) {
        for (j = 0; vals[j] != arr[i]; j++)
            ;
        start |= j << (3 * i);
        for (j = 0; j < d && vals[j] != target[i]; j++)
            ;
        if (j == d)
            return false;
        goal |= j << (3 * i);
    }

    seen = calloc((size_t)1 << (3 * n), 1);
    queue = malloc(5040 * sizeof *queue); /* 7! arrangements at most */
    seen[start] = 1;
    queue[tail++] = start;
    while (head < tail) {
        state = queue[head++];
        if (state == goal) {
            found = true;
            break;
        }
        for (i = 0; i < n; i++)
            cur[i] = (state >> (3 * i)) & 7;
        for (l = 0; l < n; l++)
            for (r = l + 1; r < n; r++) {
                next = state;
                for (k = l; k <= r; k++) {
                    next &= ~(7 << (3 * k));
                    next |= cur[l + r - k] << (3 * k);
                }
                if (!seen[next]) {
                    seen[next] = 1;
                    queue[tail++] = next;
                }
            }
    }
    free(seen);
    free(queue);
    return found;
}
