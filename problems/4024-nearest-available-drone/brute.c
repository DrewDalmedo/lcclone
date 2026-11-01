/* Slow and obviously right: try every distance from 0 upwards, and for
 * each one look through the drones in order for one at exactly that
 * distance that can make the trip. */
int nearestDrone(int **drones, int dronesSize, int *dronesColSize,
                 int *target, int targetSize)
{
    int dist, i, d;

    (void)dronesColSize;
    (void)targetSize;
    for (dist = 0; dist <= 200; dist++)
        for (i = 0; i < dronesSize; i++) {
            d = abs(drones[i][0] - target[0]) + abs(drones[i][1] - target[1]);
            if (d == dist && d <= drones[i][2])
                return i;
        }
    return -1;
}
