/* Reference solution: a closed form. With w = n / 7 full weeks and r = n % 7
 * days left over, full week k (from 0) pays 7 * k + 28, so the full weeks
 * pay 28 * w + 7 * w * (w - 1) / 2; the leftover days pay w + 1, w + 2,
 * ..., w + r, which is r * (w + 1) + r * (r - 1) / 2. */
int totalMoney(int n)
{
    int w = n / 7, r = n % 7;

    return 28 * w + 7 * (w * (w - 1) / 2) + r * (w + 1) + r * (r - 1) / 2;
}
