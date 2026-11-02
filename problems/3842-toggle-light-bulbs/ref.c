/* Reference solution: keep one on/off flag per label, flip it for each
 * entry, then read off the labels that are on from 1 to 100, which comes
 * out sorted. */
int *toggleLightBulbs(int *bulbs, int bulbsSize, int *returnSize)
{
    bool on[101] = {false};
    int *out = malloc(100 * sizeof *out);
    int i, m = 0, label;

    for (i = 0; i < bulbsSize; i++)
        on[bulbs[i]] = !on[bulbs[i]];
    for (label = 1; label <= 100; label++)
        if (on[label])
            out[m++] = label;
    *returnSize = m;
    return out;
}
