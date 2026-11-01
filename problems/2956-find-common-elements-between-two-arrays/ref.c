/* Reference solution: mark which numbers occur in each hand (numbers go
 * from 1 to 100), then count the cards of each hand whose number is
 * marked for the other hand. */
int *findIntersectionValues(int *nums1, int nums1Size, int *nums2,
                            int nums2Size, int *returnSize)
{
    bool in1[101] = {false}, in2[101] = {false};
    int *out = malloc(2 * sizeof *out), i;

    for (i = 0; i < nums1Size; i++)
        in1[nums1[i]] = true;
    for (i = 0; i < nums2Size; i++)
        in2[nums2[i]] = true;
    out[0] = 0;
    out[1] = 0;
    for (i = 0; i < nums1Size; i++)
        if (in2[nums1[i]])
            out[0]++;
    for (i = 0; i < nums2Size; i++)
        if (in1[nums2[i]])
            out[1]++;
    *returnSize = 2;
    return out;
}
