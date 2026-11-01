/* Reference solution: compare every bead with the beads before and after
 * it, wrapping the indices around the loop, and count the beads that
 * differ from both. */
int numberOfAlternatingGroups(int *colors, int colorsSize)
{
    int n = colorsSize, count = 0, i;

    for (i = 0; i < n; i++) {
        int before = colors[(i + n - 1) % n], after = colors[(i + 1) % n];

        if (colors[i] != before && colors[i] != after)
            count++;
    }
    return count;
}
