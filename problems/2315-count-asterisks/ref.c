/* Reference solution: one pass, flipping an indoors flag at every bar and
 * counting the stars met while it is off. */
int countAsterisks(char *s)
{
    int indoors = 0, count = 0;

    for (; *s; s++) {
        if (*s == '|')
            indoors = !indoors;
        else if (*s == '*' && !indoors)
            count++;
    }
    return count;
}
