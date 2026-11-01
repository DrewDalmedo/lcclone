/* Reference solution: count how many times each value 1..100 occurs, then
 * take k elements from the low end of the counts and k from the high end.
 * The back group's sum is never below the front group's, so no abs is
 * needed. */
int absDifference(int *nums, int numsSize, int k)
{
    int count[101] = {0};
    int low = 0, high = 0, left, v, take, i;

    for (i = 0; i < numsSize; i++)
        count[nums[i]]++;
    left = k;
    for (v = 1; v <= 100 && left > 0; v++) {
        take = count[v] < left ? count[v] : left;
        low += take * v;
        left -= take;
    }
    left = k;
    for (v = 100; v >= 1 && left > 0; v--) {
        take = count[v] < left ? count[v] : left;
        high += take * v;
        left -= take;
    }
    return high - low;
}
