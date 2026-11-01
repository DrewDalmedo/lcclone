/* Slow and obviously right: try every deal, giving the cards one by one
 * to the first or the second player, where a player may take a card only
 * while holding fewer than n / 2 cards and no card of the same value. */
static int deal(const int *nums, int n, int at, int *a, int na, int *b,
                int nb)
{
    int i, ok;

    if (at == n)
        return 1;
    if (na < n / 2) {
        ok = 1;
        for (i = 0; i < na; i++)
            if (a[i] == nums[at])
                ok = 0;
        if (ok) {
            a[na] = nums[at];
            if (deal(nums, n, at + 1, a, na + 1, b, nb))
                return 1;
        }
    }
    if (nb < n / 2) {
        ok = 1;
        for (i = 0; i < nb; i++)
            if (b[i] == nums[at])
                ok = 0;
        if (ok) {
            b[nb] = nums[at];
            if (deal(nums, n, at + 1, a, na, b, nb + 1))
                return 1;
        }
    }
    return 0;
}

bool isPossibleToSplit(int *nums, int numsSize)
{
    int a[50], b[50];

    return deal(nums, numsSize, 0, a, 0, b, 0) != 0;
}
