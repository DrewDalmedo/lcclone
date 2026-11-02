/* Slow and obviously right: first find where the innings ends (the tenth
 * "W", or the last code), then add up the codes up to there, looking each
 * one up in a table of all nine codes. */
int *scoreValidator(char **events, int eventsSize, int *returnSize)
{
    static const char *codes[9] = {"0", "1", "2", "3", "4", "6",
                                   "W", "WD", "NB"};
    static const int runs_of[9] = {0, 1, 2, 3, 4, 6, 0, 1, 1};
    static const int wickets_of[9] = {0, 0, 0, 0, 0, 0, 1, 0, 0};
    int *tally = malloc(2 * sizeof *tally);
    int end = eventsSize - 1, seen = 0, i, c;

    for (i = 0; i < eventsSize; i++)
        if (strcmp(events[i], "W") == 0 && ++seen == 10) {
            end = i;
            break;
        }
    tally[0] = 0;
    tally[1] = 0;
    for (i = 0; i <= end; i++)
        for (c = 0; c < 9; c++)
            if (strcmp(events[i], codes[c]) == 0) {
                tally[0] += runs_of[c];
                tally[1] += wickets_of[c];
            }
    *returnSize = 2;
    return tally;
}
