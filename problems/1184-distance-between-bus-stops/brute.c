/* Slow and obviously right: walk from start to destination one shelter
 * at a time, first with rising numbers and then with falling ones
 * (wrapping round at the ends), adding up the stretches crossed. */
int distanceBetweenBusStops(int *distance, int distanceSize, int start, int destination)
{
    int n = distanceSize, up = 0, down = 0, at;

    for (at = start; at != destination; at = (at + 1) % n)
        up += distance[at];
    for (at = start; at != destination; at = (at + n - 1) % n)
        down += distance[(at + n - 1) % n];
    return up < down ? up : down;
}
