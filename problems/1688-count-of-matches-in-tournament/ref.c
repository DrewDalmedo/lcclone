/* Reference solution: every match puts exactly one team out, and every team
 * but the champion goes out in some match, so n - 1 matches are played. */
int numberOfMatches(int n)
{
    return n - 1;
}
