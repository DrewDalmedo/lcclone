/* Reference solution: the player to move wins exactly when the square is
 * even. From an even square, moving back 1 leaves an odd one; from an odd
 * square every divisor is odd, so every move leaves an even one. The
 * player on an even square can keep it that way until the other player is
 * stuck on square 1. */
bool divisorGame(int n)
{
    return n % 2 == 0;
}
