/* Reference solution: walk the log once, keeping the number of people
 * inside (one more on 'E', one fewer on 'L') and the largest value it
 * reaches after any event. */
int minimumChairs(char *s)
{
    int inside = 0, most = 0, i;

    for (i = 0; s[i] != '\0'; i++) {
        if (s[i] == 'E')
            inside++;
        else
            inside--;
        if (inside > most)
            most = inside;
    }
    return most;
}
