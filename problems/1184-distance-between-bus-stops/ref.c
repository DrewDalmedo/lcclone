/* Reference solution: the stretches from the lower shelter number up to
 * the higher one make one way round; the rest of the loop is the other
 * way. Add up both in one pass and return the shorter. */
int distanceBetweenBusStops(int *distance, int distanceSize, int start, int destination)
{
    int lo = start < destination ? start : destination;
    int hi = start < destination ? destination : start;
    int total = 0, between = 0, i;

    for (i = 0; i < distanceSize; i++) {
        total += distance[i];
        if (i >= lo && i < hi)
            between += distance[i];
    }
    return between < total - between ? between : total - between;
}
