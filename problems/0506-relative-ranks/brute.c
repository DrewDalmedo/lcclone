/* Slow and obviously right: for each competitor, count the competitors
 * with a higher score to get the place, then spell out the label, peeling
 * the digits of a place from 4 on off by repeated division. */
char **findRelativeRanks(int *score, int scoreSize, int *returnSize)
{
    char **out = malloc((size_t)scoreSize * sizeof *out);
    char digits[16];
    int i, j, place, len, k, v;

    for (i = 0; i < scoreSize; i++) {
        char *s = calloc(16, 1);

        place = 1;
        for (j = 0; j < scoreSize; j++)
            if (score[j] > score[i])
                place++;
        if (place == 1) {
            strcpy(s, "Gold Medal");
        } else if (place == 2) {
            strcpy(s, "Silver Medal");
        } else if (place == 3) {
            strcpy(s, "Bronze Medal");
        } else {
            len = 0;
            for (v = place; v > 0; v /= 10)
                digits[len++] = (char)('0' + v % 10);
            for (k = 0; k < len; k++)
                s[k] = digits[len - 1 - k];
        }
        out[i] = s;
    }
    *returnSize = scoreSize;
    return out;
}
