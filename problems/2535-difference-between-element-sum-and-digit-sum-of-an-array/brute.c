/* Slow and obviously right: add up the numbers; separately print each one
 * as text and add up its digit characters; return the absolute difference
 * of the two totals. */
int differenceOfSum(int *nums, int numsSize)
{
    long long whole = 0, keys = 0, diff;
    char text[16];
    int i, k;

    for (i = 0; i < numsSize; i++) {
        whole += nums[i];
        snprintf(text, sizeof text, "%d", nums[i]);
        for (k = 0; text[k] != '\0'; k++)
            keys += text[k] - '0';
    }
    diff = whole - keys;
    return (int)(diff < 0 ? -diff : diff);
}
