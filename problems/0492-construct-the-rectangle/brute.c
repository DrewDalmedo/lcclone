/* Slow and obviously right: try every W from 1 to area, keep the ones that
 * divide area with W <= area / W, and remember the pair with the smallest
 * difference. */
int *constructRectangle(int area, int *returnSize)
{
    int *out = malloc(2 * sizeof *out);
    int w, l;

    out[0] = area;
    out[1] = 1;
    for (w = 1; w <= area; w++) {
        if (area % w != 0)
            continue;
        l = area / w;
        if (l >= w && l - w < out[0] - out[1]) {
            out[0] = l;
            out[1] = w;
        }
    }
    *returnSize = 2;
    return out;
}
