/* Slow and obviously right, for short rows only: try every set of bare
 * plots as the new flowers, keep the sets that leave no two flowers side
 * by side, and see whether one of them has n plots. */
bool canPlaceFlowers(int *flowerbed, int flowerbedSize, int n)
{
    unsigned long set, all = 1ul << flowerbedSize;
    int i, count, ok, full[32];

    for (set = 0; set < all; set++) {
        ok = 1;
        count = 0;
        for (i = 0; i < flowerbedSize; i++) {
            full[i] = flowerbed[i];
            if (set >> i & 1) {
                if (flowerbed[i])
                    ok = 0;
                full[i] = 1;
                count++;
            }
        }
        for (i = 0; i + 1 < flowerbedSize; i++)
            if (full[i] && full[i + 1])
                ok = 0;
        if (ok && count >= n)
            return true;
    }
    return false;
}
