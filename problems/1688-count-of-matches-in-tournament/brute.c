/* Slow and obviously right: play the rounds one by one, adding up the
 * matches of each, with the bye team going through on odd rounds. */
int numberOfMatches(int n)
{
    int teams = n, matches = 0;

    while (teams > 1) {
        matches += teams / 2;
        teams = teams / 2 + teams % 2;
    }
    return matches;
}
