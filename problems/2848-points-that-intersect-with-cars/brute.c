/* Slow and obviously right: go through the markers 1 to 100 one by one,
 * and count a marker when some car's span includes it. */
int numberOfPoints(int **nums, int numsSize, int *numsColSize)
{
    int count = 0, x, i;

    (void)numsColSize;
    for (x = 1; x <= 100; x++)
        for (i = 0; i < numsSize; i++)
            if (nums[i][0] <= x && x <= nums[i][1]) {
                count++;
                break;
            }
    return count;
}
