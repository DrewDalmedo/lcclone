/* Slow and obviously right: print each number as text, overwrite every
 * character with the largest one, and read the text back as a number. */
int sumOfEncryptedInt(int *nums, int numsSize)
{
    char text[16];
    char high;
    int sum = 0, i, k;

    for (i = 0; i < numsSize; i++) {
        snprintf(text, sizeof text, "%d", nums[i]);
        high = '0';
        for (k = 0; text[k]; k++)
            if (text[k] > high)
                high = text[k];
        for (k = 0; text[k]; k++)
            text[k] = high;
        sum += atoi(text);
    }
    return sum;
}
