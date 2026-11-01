/* Slow and obviously right: for every place to cut, count the 0s of the
 * front piece and the 1s of the back piece from scratch. */
int maxScore(char *s)
{
    int n = (int)strlen(s), best = -1, cut, score, i;

    for (cut = 1; cut < n; cut++) {
        score = 0;
        for (i = 0; i < cut; i++)
            if (s[i] == '0')
                score++;
        for (i = cut; i < n; i++)
            if (s[i] == '1')
                score++;
        if (score > best)
            best = score;
    }
    return best;
}
