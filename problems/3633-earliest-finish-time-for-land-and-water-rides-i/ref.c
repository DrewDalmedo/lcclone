/* Reference solution: the finish time only grows with the time the first
 * ride ends, so whichever kind goes first, its best ride is the one that
 * ends earliest. For each order, take that end and try every ride of the
 * other kind after it. O(n + m). */
static int best_after(int end, int *start, int *dur, int count)
{
    int best = INT_MAX, i, t;

    for (i = 0; i < count; i++) {
        t = (start[i] > end ? start[i] : end) + dur[i];
        if (t < best)
            best = t;
    }
    return best;
}

static int earliest_end(int *start, int *dur, int count)
{
    int best = INT_MAX, i;

    for (i = 0; i < count; i++)
        if (start[i] + dur[i] < best)
            best = start[i] + dur[i];
    return best;
}

int earliestFinishTime(int *landStartTime, int landStartTimeSize,
                       int *landDuration, int landDurationSize,
                       int *waterStartTime, int waterStartTimeSize,
                       int *waterDuration, int waterDurationSize)
{
    int land_end = earliest_end(landStartTime, landDuration,
                                landStartTimeSize);
    int water_end = earliest_end(waterStartTime, waterDuration,
                                 waterStartTimeSize);
    int land_first = best_after(land_end, waterStartTime, waterDuration,
                                waterStartTimeSize);
    int water_first = best_after(water_end, landStartTime, landDuration,
                                 landStartTimeSize);

    (void)landDurationSize;
    (void)waterDurationSize;
    return land_first < water_first ? land_first : water_first;
}
