/* Slow and obviously right, straight from the definition: write down every
 * push time, find the greatest, then take the smallest label among all the
 * pushes that have it. */
int buttonWithLongestTime(int **events, int eventsSize, int *eventsColSize)
{
    int *t = malloc((size_t)eventsSize * sizeof *t);
    int i, longest = 0, label = 0;

    (void)eventsColSize;
    for (i = 0; i < eventsSize; i++)
        t[i] = events[i][1] - (i == 0 ? 0 : events[i - 1][1]);
    for (i = 0; i < eventsSize; i++)
        if (t[i] > longest)
            longest = t[i];
    for (i = 0; i < eventsSize; i++)
        if (t[i] == longest && (label == 0 || events[i][0] < label))
            label = events[i][0];
    free(t);
    return label;
}
