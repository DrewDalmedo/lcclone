/* Slow and obviously right: try both claims. For each, deal every card by
 * counting its digits, add up both hands, and see whether Alice's is
 * higher. */
static int digits(int v)
{
    int d = 0;

    do {
        d++;
        v /= 10;
    } while (v > 0);
    return d;
}

bool canAliceWin(int *nums, int numsSize)
{
    int claim, alice, bob, i;

    for (claim = 1; claim <= 2; claim++) {
        alice = 0;
        bob = 0;
        for (i = 0; i < numsSize; i++) {
            if (digits(nums[i]) == claim)
                alice += nums[i];
            else
                bob += nums[i];
        }
        if (alice > bob)
            return true;
    }
    return false;
}
