/* Slow and obviously right: a breadth-first search over the arrays that
 * trims can reach, trying every allowed h from k up to the tallest height
 * minus 1. A tower below k can never come back up (trims only lower), so
 * such an array, and any h below k, is a dead end. */
static int seen_state(const int *states, int count, const int *s, int n)
{
    int i;

    for (i = 0; i < count; i++)
        if (memcmp(states + (size_t)i * n, s, (size_t)n * sizeof *s) == 0)
            return 1;
    return 0;
}

int minOperations(int *nums, int numsSize, int k)
{
    int n = numsSize, cap = 64, count = 1, head = 0, answer = -1;
    int i, h, top, ok, done;
    int *states, *dist, *next, *cur;

    for (i = 0; i < n; i++)
        if (nums[i] < k)
            return -1;
    states = malloc((size_t)cap * n * sizeof *states);
    dist = malloc((size_t)cap * sizeof *dist);
    next = malloc((size_t)n * sizeof *next);
    memcpy(states, nums, (size_t)n * sizeof *nums);
    dist[0] = 0;
    while (head < count) {
        cur = states + (size_t)head * n;
        done = 1;
        top = cur[0];
        for (i = 0; i < n; i++) {
            if (cur[i] != k)
                done = 0;
            if (cur[i] > top)
                top = cur[i];
        }
        if (done) {
            answer = dist[head];
            break;
        }
        for (h = k; h < top; h++) {
            ok = 1;
            for (i = 0; i < n; i++)
                if (cur[i] > h && cur[i] != top)
                    ok = 0;
            if (!ok)
                continue;
            for (i = 0; i < n; i++)
                next[i] = cur[i] > h ? h : cur[i];
            if (seen_state(states, count, next, n))
                continue;
            if (count == cap) {
                cap *= 2;
                states = realloc(states, (size_t)cap * n * sizeof *states);
                dist = realloc(dist, (size_t)cap * sizeof *dist);
                cur = states + (size_t)head * n;
            }
            memcpy(states + (size_t)count * n, next, (size_t)n * sizeof *next);
            dist[count++] = dist[head] + 1;
        }
        head++;
    }
    free(states);
    free(dist);
    free(next);
    return answer;
}
