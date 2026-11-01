/* Slow and obviously right: check the array, then shift it right one
 * place at a time and check it again, n - 1 times in all; n shifts would
 * bring back the starting array. */
int minimumRightShifts(int *nums, int numsSize)
{
    int *a = malloc((size_t)numsSize * sizeof *a);
    int shifts, sorted, last, i, answer = -1;

    memcpy(a, nums, (size_t)numsSize * sizeof *a);
    for (shifts = 0; shifts < numsSize; shifts++) {
        sorted = 1;
        for (i = 0; i + 1 < numsSize; i++)
            if (a[i] >= a[i + 1])
                sorted = 0;
        if (sorted) {
            answer = shifts;
            break;
        }
        last = a[numsSize - 1];
        for (i = numsSize - 1; i > 0; i--)
            a[i] = a[i - 1];
        a[0] = last;
    }
    free(a);
    return answer;
}
