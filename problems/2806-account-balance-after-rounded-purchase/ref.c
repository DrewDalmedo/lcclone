/* Reference solution: adding 5 before the integer division by 10 rounds
 * to the nearest multiple of 10, with a last digit of 5 going up; take
 * that multiple off the balance of 100. */
int accountBalanceAfterPurchase(int purchaseAmount)
{
    return 100 - (purchaseAmount + 5) / 10 * 10;
}
