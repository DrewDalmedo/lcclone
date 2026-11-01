/* Slow and obviously right: keep the counter's whole value as a decimal
 * number, one digit per array element (least significant first), double it
 * and add each bit by hand, and test the last decimal digit: a number is a
 * multiple of 5 exactly when it ends in 0 or 5. */
bool *prefixesDivBy5(int *nums, int numsSize, int *returnSize)
{
    bool *answer = malloc((size_t)numsSize * sizeof *answer);
    /* The value stays below 2^n, which has at most n decimal digits. */
    int cap = numsSize + 1, len = 1, i, k, carry, d;
    int *digit = calloc((size_t)cap, sizeof *digit);

    for (i = 0; i < numsSize; i++) {
        carry = nums[i];
        for (k = 0; k < len; k++) {
            d = digit[k] * 2 + carry;
            digit[k] = d % 10;
            carry = d / 10;
        }
        if (carry)
            digit[len++] = carry;
        answer[i] = digit[0] == 0 || digit[0] == 5;
    }
    free(digit);
    *returnSize = numsSize;
    return answer;
}
