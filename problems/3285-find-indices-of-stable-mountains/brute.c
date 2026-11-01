/* Slow and obviously right: for every mountain, look through all the
 * others for the one standing right before it, and check that one's
 * height. */
int *stableMountains(int *height, int heightSize, int threshold,
                     int *returnSize)
{
    int *out = malloc((size_t)heightSize * sizeof *out), i, j, k = 0;

    for (i = 0; i < heightSize; i++) {
        int stable = 0;

        for (j = 0; j < heightSize; j++)
            if (j + 1 == i && height[j] > threshold)
                stable = 1;
        if (stable)
            out[k++] = i;
    }
    *returnSize = k;
    return out;
}
