/* Slow and obviously right: mark every second some strike poisons, in a
 * table running up to the last strike's last second, and count the marks. */
int findPoisonedDuration(int *timeSeries, int timeSeriesSize, int duration)
{
    int end = timeSeries[timeSeriesSize - 1] + duration, count = 0, i, s;
    char *poisoned = calloc((size_t)end + 1, 1);

    for (i = 0; i < timeSeriesSize; i++)
        for (s = timeSeries[i]; s < timeSeries[i] + duration; s++)
            poisoned[s] = 1;
    for (s = 0; s < end; s++)
        count += poisoned[s];
    free(poisoned);
    return count;
}
