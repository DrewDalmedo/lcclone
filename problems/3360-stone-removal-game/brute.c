/* Slow and obviously right: straight from the rules, the player facing
 * `stones` with quota `quota` wins when they can move and the opponent,
 * facing what is left with a quota one less, then loses. */
static bool mover_wins(int stones, int quota)
{
    if (quota <= 0 || stones < quota)
        return false;
    return !mover_wins(stones - quota, quota - 1);
}

bool canAliceWin(int n)
{
    return mover_wins(n, 10);
}
