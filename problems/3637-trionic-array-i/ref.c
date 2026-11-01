/* Reference solution: one walk. Climb as far as the values rise, then
 * descend as far as they fall, then climb again; each stretch must take
 * at least one step, and the last climb must reach the end. */
bool isTrionic(int *nums, int numsSize)
{
    int i = 0, p, q;

    while (i + 1 < numsSize && nums[i] < nums[i + 1])
        i++;
    p = i;
    if (p == 0)
        return false;
    while (i + 1 < numsSize && nums[i] > nums[i + 1])
        i++;
    q = i;
    if (q == p || q == numsSize - 1)
        return false;
    while (i + 1 < numsSize && nums[i] < nums[i + 1])
        i++;
    return i == numsSize - 1;
}
