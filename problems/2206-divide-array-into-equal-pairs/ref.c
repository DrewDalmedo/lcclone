/* Reference solution: the socks of one colour can all be bundled exactly
 * when there is an even number of them. Keep one parity flag per colour
 * (1 to 500, so 501 slots), flip it for every sock, and check that every
 * flag ends up clear. */
bool divideArray(int *nums, int numsSize)
{
    bool odd[501] = {false};
    int i;

    for (i = 0; i < numsSize; i++)
        odd[nums[i]] = !odd[nums[i]];
    for (i = 1; i <= 500; i++)
        if (odd[i])
            return false;
    return true;
}
