/* Reference solution: each round can close the gap between the markers by
 * at most 2 (the upper one steps down while the lower one steps up), so
 * the farthest start above num that still meets within t rounds is
 * num + 2 * t. */
int theMaximumAchievableX(int num, int t)
{
    return num + 2 * t;
}
