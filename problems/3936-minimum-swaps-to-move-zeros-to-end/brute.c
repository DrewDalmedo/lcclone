/* Slow and obviously right: only which positions hold a zero matters, so
 * an arrangement is a bit mask (bit i set when position i holds a zero).
 * Search breadth-first from the starting mask, a swap of any zero with any
 * non-zero value being one step, until a settled mask is reached. Small
 * cases only: the masks of up to 16 positions are searched. */
int minimumSwaps(int *nums, int numsSize)
{
    int n = numsSize, start = 0, i, j, head = 0, tail = 0, mask, next;
    int z = 0, goal, *dist, *queue, answer = -1;

    if (n > 16)
        return -1;
    for (i = 0; i < n; i++)
        if (nums[i] == 0) {
            start |= 1 << i;
            z++;
        }
    goal = ((1 << z) - 1) << (n - z);
    dist = malloc(sizeof *dist << n);
    queue = malloc(sizeof *queue << n);
    for (i = 0; i < 1 << n; i++)
        dist[i] = -1;
    dist[start] = 0;
    queue[tail++] = start;
    while (head < tail) {
        mask = queue[head++];
        if (mask == goal) {
            answer = dist[mask];
            break;
        }
        for (i = 0; i < n; i++)
            for (j = 0; j < n; j++) {
                if (!(mask >> i & 1) || (mask >> j & 1))
                    continue;
                next = mask ^ (1 << i) ^ (1 << j);
                if (dist[next] < 0) {
                    dist[next] = dist[mask] + 1;
                    queue[tail++] = next;
                }
            }
    }
    free(dist);
    free(queue);
    return answer;
}
