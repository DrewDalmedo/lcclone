/* Slow and obviously right: try 1, 2, 3, ... chairs, replaying the log
 * with that many each time, and return the first number with which no
 * arrival finds every chair taken. */
int minimumChairs(char *s)
{
    int chairs, free_chairs, ok, i;

    for (chairs = 1;; chairs++) {
        free_chairs = chairs;
        ok = 1;
        for (i = 0; s[i] != '\0' && ok; i++) {
            if (s[i] == 'E') {
                if (free_chairs == 0)
                    ok = 0;
                else
                    free_chairs--;
            } else {
                free_chairs++;
            }
        }
        if (ok)
            return chairs;
    }
}
