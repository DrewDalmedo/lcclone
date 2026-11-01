/* Reference solution: find r, the integer square root of area, then walk
 * W down from r to the first divisor of area. That is the largest W with
 * W <= L = area / W, which leaves the smallest difference L - W. The walk
 * ends at W = 1 at the latest. */
int *constructRectangle(int area, int *returnSize)
{
    int *out = malloc(2 * sizeof *out);
    int w = 1;

    while ((long long)(w + 1) * (w + 1) <= area)
        w++;
    while (area % w != 0)
        w--;
    out[0] = area / w;
    out[1] = w;
    *returnSize = 2;
    return out;
}
