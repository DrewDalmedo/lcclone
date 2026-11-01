/* Reference solution: fan k makes t = tickets[k] visits. Before k's last
 * sale, a fan at or ahead of k in the starting order has visited at most t
 * times, and a fan behind k at most t - 1 times; each visit is a second. */
int timeRequiredToBuy(int *tickets, int ticketsSize, int k)
{
    int t = tickets[k], total = 0, i, cap;

    for (i = 0; i < ticketsSize; i++) {
        cap = i <= k ? t : t - 1;
        total += tickets[i] < cap ? tickets[i] : cap;
    }
    return total;
}
