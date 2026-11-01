/* Reference solution: one pass over the drones, skipping those whose
 * Manhattan distance to the target is over their range, and keeping the
 * first one at the smallest distance. */
int nearestDrone(int **drones, int dronesSize, int *dronesColSize,
                 int *target, int targetSize)
{
    int best = -1, bestDist = 0, i, dx, dy, d;

    (void)dronesColSize;
    (void)targetSize;
    for (i = 0; i < dronesSize; i++) {
        dx = drones[i][0] - target[0];
        dy = drones[i][1] - target[1];
        d = (dx < 0 ? -dx : dx) + (dy < 0 ? -dy : dy);
        if (d > drones[i][2])
            continue;
        if (best < 0 || d < bestDist) {
            best = i;
            bestDist = d;
        }
    }
    return best;
}
