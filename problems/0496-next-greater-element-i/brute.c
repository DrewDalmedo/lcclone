/* Slow and obviously right: for each number of nums1, find its position in
 * nums2 and read on to the right until a greater number turns up. */
int *nextGreaterElement(int *nums1, int nums1Size, int *nums2, int nums2Size,
                        int *returnSize)
{
    int *answer = malloc((size_t)nums1Size * sizeof *answer);
    int i, j, k;

    for (i = 0; i < nums1Size; i++) {
        for (j = 0; nums2[j] != nums1[i]; j++)
            ;
        answer[i] = -1;
        for (k = j + 1; k < nums2Size; k++)
            if (nums2[k] > nums2[j]) {
                answer[i] = nums2[k];
                break;
            }
    }
    *returnSize = nums1Size;
    return answer;
}
