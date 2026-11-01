/* Reference solution: nobody has a choice, so count the turns that can be
 * taken, removing quotas 10, 9, 8, ... while the heap holds enough. Alice
 * takes the odd-numbered turns, so she wins when that count is odd. */
bool canAliceWin(int n)
{
    int quota = 10, turns = 0;

    while (quota > 0 && n >= quota) {
        n -= quota;
        quota--;
        turns++;
    }
    return turns % 2 == 1;
}
