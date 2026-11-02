/* Reference solution: one pass from left to right, keeping the index of
 * the smallest box seen so far that the item fits in. A later box replaces
 * it only when strictly smaller, so the lowest index wins a tie. */
int minimumIndex(int *capacity, int capacitySize, int itemSize)
{
    int best = -1, i;

    for (i = 0; i < capacitySize; i++)
        if (capacity[i] >= itemSize &&
            (best == -1 || capacity[i] < capacity[best]))
            best = i;
    return best;
}
