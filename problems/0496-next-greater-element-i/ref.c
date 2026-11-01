/* Reference solution: walk nums2 from left to right with a stack of the
 * numbers still waiting for a greater one (they decrease from bottom to
 * top). Each new number settles every smaller one on top of the stack, and
 * a table indexed by value keeps the answers for the lookups of nums1. */
int *nextGreaterElement(int *nums1, int nums1Size, int *nums2, int nums2Size,
                        int *returnSize)
{
    int *next = malloc(10001 * sizeof *next);
    int *stack = malloc((size_t)nums2Size * sizeof *stack);
    int *answer = malloc((size_t)nums1Size * sizeof *answer);
    int top = 0, i;

    for (i = 0; i <= 10000; i++)
        next[i] = -1;
    for (i = 0; i < nums2Size; i++) {
        while (top > 0 && stack[top - 1] < nums2[i])
            next[stack[--top]] = nums2[i];
        stack[top++] = nums2[i];
    }
    for (i = 0; i < nums1Size; i++)
        answer[i] = next[nums1[i]];
    free(stack);
    free(next);
    *returnSize = nums1Size;
    return answer;
}
