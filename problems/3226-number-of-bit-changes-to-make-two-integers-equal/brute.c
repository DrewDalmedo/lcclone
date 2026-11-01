/* Slow and obviously right: breadth-first search over the numbers reachable
 * from n, one move (clearing one 1-bit) at a time, until k turns up. */
int minChanges(int n, int k)
{
    int size = 1 << 20;
    int *dist = malloc((size_t)size * sizeof *dist);
    int *queue = malloc((size_t)size * sizeof *queue);
    int head = 0, tail = 0, v, b, w, answer = -1, i;

    for (i = 0; i < size; i++)
        dist[i] = -1;
    dist[n] = 0;
    queue[tail++] = n;
    while (head < tail) {
        v = queue[head++];
        if (v == k) {
            answer = dist[v];
            break;
        }
        for (b = 0; b < 20; b++) {
            if (!(v & (1 << b)))
                continue;
            w = v - (1 << b);
            if (dist[w] < 0) {
                dist[w] = dist[v] + 1;
                queue[tail++] = w;
            }
        }
    }
    free(queue);
    free(dist);
    return answer;
}
