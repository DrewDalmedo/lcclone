/* Reference solution: one pass from mountain 1, keeping each index whose
 * left neighbour is taller than the threshold. */
int *stableMountains(int *height, int heightSize, int threshold,
                     int *returnSize)
{
    int *out = malloc((size_t)heightSize * sizeof *out), i, k = 0;

    for (i = 1; i < heightSize; i++)
        if (height[i - 1] > threshold)
            out[k++] = i;
    *returnSize = k;
    return out;
}
