/* Reference solution: a deal exists exactly when no value is on more than
 * two cards. Two equal cards go one to each player, and the cards whose
 * value is on no other card then fill both hands up to n / 2. So count
 * the cards of each value. */
bool isPossibleToSplit(int *nums, int numsSize)
{
    int count[101] = {0}, i;

    for (i = 0; i < numsSize; i++)
        if (++count[nums[i]] > 2)
            return false;
    return true;
}
