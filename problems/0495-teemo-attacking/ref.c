/* Reference solution: one pass over neighbouring strikes. A strike adds
 * the seconds from its own time up to the next strike's, but never more
 * than duration of them; the last strike adds all duration seconds. The
 * total is at most 10^7 + 10^7, so an int holds it. */
int findPoisonedDuration(int *timeSeries, int timeSeriesSize, int duration)
{
    int total = 0, gap, i;

    for (i = 0; i + 1 < timeSeriesSize; i++) {
        gap = timeSeries[i + 1] - timeSeries[i];
        total += gap < duration ? gap : duration;
    }
    return total + duration;
}
