/* Slow and obviously right: list every row that jumps can make from s1,
 * making each jump from each row found until no new row turns up, do the
 * same from s2, and look for a row in both lists. */
static int reach(const char *start, char rows[][5])
{
    int count = 1, k, i, j;
    char t[5], c;

    memcpy(rows[0], start, 5);
    for (k = 0; k < count; k++) {
        for (i = 0; i + 2 < 4; i++) {
            memcpy(t, rows[k], 5);
            c = t[i];
            t[i] = t[i + 2];
            t[i + 2] = c;
            for (j = 0; j < count && strcmp(rows[j], t) != 0; j++)
                ;
            if (j == count)
                memcpy(rows[count++], t, 5);
        }
    }
    return count;
}

bool canBeEqual(char *s1, char *s2)
{
    char a[24][5], b[24][5];
    int na = reach(s1, a), nb = reach(s2, b), i, j;

    for (i = 0; i < na; i++)
        for (j = 0; j < nb; j++)
            if (strcmp(a[i], b[j]) == 0)
                return true;
    return false;
}
