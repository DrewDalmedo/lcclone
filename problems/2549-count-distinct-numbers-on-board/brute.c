/* Slow and obviously right: play out the days one by one, each house that
 * knew the news at the start of the day telling every house i it should.
 * Once a day tells nobody new, no later day can, so stop there. */
int distinctIntegers(int n)
{
    bool knows[101] = {false}, next[101];
    int known = 1, grew = 1, x, i;

    knows[n] = true;
    while (grew) {
        grew = 0;
        memcpy(next, knows, sizeof next);
        for (x = 1; x <= n; x++) {
            if (!knows[x])
                continue;
            for (i = 1; i <= n; i++) {
                if (x % i == 1 && !next[i]) {
                    next[i] = true;
                    known++;
                    grew = 1;
                }
            }
        }
        memcpy(knows, next, sizeof knows);
    }
    return known;
}
