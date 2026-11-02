/* Slow and obviously right: try the sizes from itemSize up to the largest
 * possible capacity, and return the first box, from the left, that has
 * the size being tried. */
int minimumIndex(int *capacity, int capacitySize, int itemSize)
{
    int size, i;

    for (size = itemSize; size <= 100; size++)
        for (i = 0; i < capacitySize; i++)
            if (capacity[i] == size)
                return i;
    return -1;
}
