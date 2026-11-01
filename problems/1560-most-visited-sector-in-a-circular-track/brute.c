/* Slow and obviously right: walk the whole patrol one step at a time,
 * counting the visits to every room, then list the rooms whose count is
 * the largest. */
int *mostVisited(int n, int *rounds, int roundsSize, int *returnSize)
{
    int visits[101] = {0};
    int *out = malloc((size_t)n * sizeof *out);
    int room = rounds[0], best = 0, k = 0, i;

    visits[room]++;
    for (i = 1; i < roundsSize; i++)
        while (room != rounds[i]) {
            room = room == n ? 1 : room + 1;
            visits[room]++;
        }
    for (i = 1; i <= n; i++)
        if (visits[i] > best)
            best = visits[i];
    for (i = 1; i <= n; i++)
        if (visits[i] == best)
            out[k++] = i;
    *returnSize = k;
    return out;
}
