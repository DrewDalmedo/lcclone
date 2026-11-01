/* Reference solution: a pile that is a multiple of 4 is lost for the
 * player to move, since every move leaves a pile that isn't one; from any
 * other pile, taking n % 4 pebbles leaves a multiple of 4 (0 included,
 * when that move takes the last pebble). n is positive, so % is safe. */
bool canWinNim(int n)
{
    return n % 4 != 0;
}
