/* Reference solution: one pass over the record, timing each push from the
 * one before it (push 0 from second 0, so it seeds the best). A push
 * replaces the best so far when it waited longer, or exactly as long on a
 * smaller label. */
int buttonWithLongestTime(int **events, int eventsSize, int *eventsColSize)
{
    int best = events[0][0], longest = events[0][1], wait, i;

    (void)eventsColSize;
    for (i = 1; i < eventsSize; i++) {
        wait = events[i][1] - events[i - 1][1];
        if (wait > longest || (wait == longest && events[i][0] < best)) {
            longest = wait;
            best = events[i][0];
        }
    }
    return best;
}
