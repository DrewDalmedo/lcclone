/* Reference solution: count the 1s of s first. Then move the cut from
 * after the first character to before the last, keeping the 0s and 1s of
 * the front piece: the score is the front's 0s plus the 1s not in it. */
int maxScore(char *s)
{
    int n = (int)strlen(s), ones = 0, zerosFront = 0, onesFront = 0;
    int best = -1, score, i;

    for (i = 0; i < n; i++)
        if (s[i] == '1')
            ones++;
    for (i = 0; i < n - 1; i++) {
        if (s[i] == '0')
            zerosFront++;
        else
            onesFront++;
        score = zerosFront + ones - onesFront;
        if (score > best)
            best = score;
    }
    return best;
}
