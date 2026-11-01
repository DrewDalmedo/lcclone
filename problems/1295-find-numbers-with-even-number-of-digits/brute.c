/* Slow and obviously right: print each number as text and look at the
 * length of the text. */
int findNumbers(int *nums, int numsSize)
{
    char text[16];
    int count = 0, i;

    for (i = 0; i < numsSize; i++) {
        snprintf(text, sizeof text, "%d", nums[i]);
        if (strlen(text) % 2 == 0)
            count++;
    }
    return count;
}
