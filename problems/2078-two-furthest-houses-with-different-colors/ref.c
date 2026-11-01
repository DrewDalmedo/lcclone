/* Reference solution: some farthest pair always includes an end house.
 * Pair the first house with the last house of another color, pair the last
 * house with the first house of another color, and keep the wider pair. */
int maxDistance(int *colors, int colorsSize)
{
    int n = colorsSize, left = 0, right = n - 1;

    while (colors[right] == colors[0])
        right--;
    while (colors[left] == colors[n - 1])
        left++;
    return right > n - 1 - left ? right : n - 1 - left;
}
