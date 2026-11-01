/* Slow and obviously right: go through the socks in order, and give each
 * sock not yet bundled a partner, the next unbundled sock of the same
 * colour. Socks of one colour are interchangeable, so this fails only when
 * some sock has no partner at all. */
bool divideArray(int *nums, int numsSize)
{
    bool used[1000] = {false};
    int i, j;

    for (i = 0; i < numsSize; i++) {
        if (used[i])
            continue;
        for (j = i + 1; j < numsSize; j++)
            if (!used[j] && nums[j] == nums[i])
                break;
        if (j == numsSize)
            return false;
        used[i] = used[j] = true;
    }
    return true;
}
