/* Reference solution: a jump moves a letter two places, so letters at even
 * positions stay at even positions, and those at odd ones at odd ones. The
 * rows can be matched exactly when their even positions hold the same two
 * letters, in either order, and so do their odd positions. */
static bool same_pair(char a, char b, char c, char d)
{
    return (a == c && b == d) || (a == d && b == c);
}

bool canBeEqual(char *s1, char *s2)
{
    return same_pair(s1[0], s1[2], s2[0], s2[2]) &&
           same_pair(s1[1], s1[3], s2[1], s2[3]);
}
