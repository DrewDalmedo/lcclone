/* Reference solution: go along the row and plant in every bare plot whose
 * neighbours are bare, counting the new flowers, until there are n.
 * Planting as early as possible never costs a place further on, so this
 * fits the most flowers. The row itself is left as it was: before tells
 * whether the plot just passed holds a flower, old or new. */
bool canPlaceFlowers(int *flowerbed, int flowerbedSize, int n)
{
    int planted = 0, before = 0, i;

    for (i = 0; i < flowerbedSize && planted < n; i++) {
        if (flowerbed[i]) {
            before = 1;
        } else if (!before &&
                   (i + 1 == flowerbedSize || !flowerbed[i + 1])) {
            planted++;
            before = 1;
        } else {
            before = 0;
        }
    }
    return planted >= n;
}
