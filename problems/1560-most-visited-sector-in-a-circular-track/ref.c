/* Reference solution: all the stretches together are one walk from
 * s = rounds[0] on to e = rounds[m]. Its full turns of the ring add the
 * same number of visits to every room, so the rooms visited most are those
 * on the last part of the walk, from s to e: s..e when s <= e, and 1..e
 * followed by s..n when the walk passes room n. */
int *mostVisited(int n, int *rounds, int roundsSize, int *returnSize)
{
    int s = rounds[0], e = rounds[roundsSize - 1], k = 0, room;
    int *out = malloc((size_t)n * sizeof *out);

    if (s <= e) {
        for (room = s; room <= e; room++)
            out[k++] = room;
    } else {
        for (room = 1; room <= e; room++)
            out[k++] = room;
        for (room = s; room <= n; room++)
            out[k++] = room;
    }
    *returnSize = k;
    return out;
}
