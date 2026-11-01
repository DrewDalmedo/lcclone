/* Reference solution: one pass keeping the biggest digit seen so far and
 * the biggest one below it, both -1 until found. A new biggest pushes the
 * old one down to second place; another copy of the biggest changes
 * nothing. */
int secondHighest(char *s)
{
    int first = -1, second = -1, d;

    for (; *s; s++) {
        if (*s < '0' || *s > '9')
            continue;
        d = *s - '0';
        if (d > first) {
            second = first;
            first = d;
        } else if (d < first && d > second) {
            second = d;
        }
    }
    return second;
}
