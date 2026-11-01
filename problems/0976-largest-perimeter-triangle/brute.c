/* Slow and obviously right: try every three positions, keep the triples
 * where each side is shorter than the other two together, and return the
 * largest total. */
int largestPerimeter(int *nums, int numsSize)
{
    int best = 0, i, j, k, a, b, c;

    for (i = 0; i < numsSize; i++)
        for (j = i + 1; j < numsSize; j++)
            for (k = j + 1; k < numsSize; k++) {
                a = nums[i];
                b = nums[j];
                c = nums[k];
                if (a < b + c && b < a + c && c < a + b && a + b + c > best)
                    best = a + b + c;
            }
    return best;
}
