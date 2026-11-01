/* Reference solution: every space joins the last letter of one word to
 * the first letter of the next, so compare the letters on both sides of
 * each space, then the first letter of the sentence with its last. */
bool isCircularSentence(char *sentence)
{
    size_t m = strlen(sentence), p;

    if (sentence[0] != sentence[m - 1])
        return false;
    for (p = 1; p + 1 < m; p++)
        if (sentence[p] == ' ' && sentence[p - 1] != sentence[p + 1])
            return false;
    return true;
}
