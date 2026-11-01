/* Reference solution: one pass that stops after the tenth wicket. A code
 * is a wicket when it is "W" alone; "WD" and "NB" add one run; any other
 * code is a single digit, the runs it adds. */
int *scoreValidator(char **events, int eventsSize, int *returnSize)
{
    int *tally = malloc(2 * sizeof *tally);
    int runs = 0, wickets = 0, i;
    const char *e;

    for (i = 0; i < eventsSize && wickets < 10; i++) {
        e = events[i];
        if (e[0] == 'W' && e[1] == '\0')
            wickets++;
        else if (e[0] == 'W' || e[0] == 'N')
            runs++;
        else
            runs += e[0] - '0';
    }
    tally[0] = runs;
    tally[1] = wickets;
    *returnSize = 2;
    return tally;
}
