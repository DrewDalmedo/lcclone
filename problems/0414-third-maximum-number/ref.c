/* Reference solution: one pass that keeps the three largest different
 * values seen so far. They are kept in long long, with LLONG_MIN for "none
 * yet", so that INT_MIN can be a real value. */
int thirdMax(int *nums, int numsSize)
{
    long long first = LLONG_MIN, second = LLONG_MIN, third = LLONG_MIN, v;
    int i;

    for (i = 0; i < numsSize; i++) {
        v = nums[i];
        if (v == first || v == second || v == third)
            continue;
        if (v > first) {
            third = second;
            second = first;
            first = v;
        } else if (v > second) {
            third = second;
            second = v;
        } else if (v > third) {
            third = v;
        }
    }
    return (int)(third == LLONG_MIN ? first : third);
}
