/* Slow and obviously right: try every set of boxes, and keep the smallest
 * one whose capacities add up to at least the number of apples. */
int minimumBoxes(int *apple, int appleSize, int *capacity, int capacitySize)
{
    int total = 0, best = capacitySize, room, size, i;
    unsigned long mask;

    for (i = 0; i < appleSize; i++)
        total += apple[i];
    for (mask = 1; mask < 1UL << capacitySize; mask++) {
        room = 0;
        size = 0;
        for (i = 0; i < capacitySize; i++)
            if (mask >> i & 1) {
                room += capacity[i];
                size++;
            }
        if (room >= total && size < best)
            best = size;
    }
    return best;
}
